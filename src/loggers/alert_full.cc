/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
** Copyright (C) 2002-2013 Sourcefire, Inc.
** Copyright (C) 1998-2002 Martin Roesch <roesch@sourcefire.com>
** Copyright (C) 2000,2001 Andrew R. Baker <andrewb@uab.edu>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/* alert_full
 *
 * Purpose:  output plugin for full alerting
 *
 * Arguments:  alert file (eventually)
 *
 * Effect:
 *
 * Alerts are written to a file in the snort full alert format
 *
 * Comments:   Allows use of full alerts with other output plugin types
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "snort_types.h"
#include "framework/logger.h"
#include "framework/module.h"
#include "event.h"
#include "protocols/packet.h"
#include "snort_debug.h"
#include "parser.h"
#include "util.h"
#include "mstring.h"
#include "snort.h"
#include "sf_textlog.h"
#include "log_text.h"
#include "packet_io/sfdaq.h"
#include "packet_io/intf.h"

static THREAD_LOCAL TextLog* full_log = nullptr;

#define LOG_BUFFER (4*K_BYTES)

using namespace std;

//-------------------------------------------------------------------------
// module stuff
//-------------------------------------------------------------------------

static const Parameter full_params[] =
{
    { "file", Parameter::PT_STRING, nullptr, nullptr,
      "name of alert file" },

    { "limit", Parameter::PT_INT, "0:", "0",
      "set limit (0 is unlimited)" },

    { "units", Parameter::PT_ENUM, "B | K | M | G", "B",
      "limit is in bytes | KB | MB | GB" },

    { nullptr, Parameter::PT_MAX, nullptr, nullptr, nullptr }
};

class FullModule : public Module
{
public:
    FullModule() : Module("alert_full", full_params) { };
    bool set(const char*, Value&, SnortConfig*);
    bool begin(const char*, int, SnortConfig*);
    bool end(const char*, int, SnortConfig*);

public:
    string file;
    unsigned long limit;
    unsigned units;
};

bool FullModule::set(const char*, Value& v, SnortConfig*)
{
    if ( v.is("file") )
        file = v.get_string();

    else if ( v.is("limit") )
        limit = v.get_long();

    else if ( v.is("units") )
        units = v.get_long();

    else
        return false;

    return true;
}

bool FullModule::begin(const char*, int, SnortConfig*)
{
    file = "stdout";
    limit = 0;
    units = 0;
    return true;
}

bool FullModule::end(const char*, int, SnortConfig*)
{
    while ( units-- )
        limit *= 1024;

    return true;
}

//-------------------------------------------------------------------------
// logger stuff
//-------------------------------------------------------------------------

class FullLogger : public Logger {
public:
    FullLogger(FullModule*);

    void open();
    void close();

    void alert(Packet*, const char* msg, Event*);

private:
    string file;
    unsigned long limit;
};

FullLogger::FullLogger(FullModule* m)
{
    file = m->file;
    limit = m->limit;
}

void FullLogger::open()
{
    full_log = TextLog_Init(file.c_str(), LOG_BUFFER, limit);
}

void FullLogger::close()
{
    if ( full_log )
        TextLog_Term(full_log);
}

void FullLogger::alert(Packet *p, const char *msg, Event *event)
{
    {
        TextLog_Puts(full_log, "[**] ");

        if(event != NULL)
        {
                TextLog_Print(full_log, "[%lu:%lu:%lu] ",
                        (unsigned long) event->sig_info->generator,
                        (unsigned long) event->sig_info->id,
                        (unsigned long) event->sig_info->rev);
        }

        if (ScAlertInterface())
        {
            const char* iface = PRINT_INTERFACE(DAQ_GetInterfaceSpec());
            TextLog_Print(full_log, " <%s> ", iface);
        }

        if(msg != NULL)
        {
            TextLog_Puts(full_log, msg);
            TextLog_Puts(full_log, " [**]\n");
        }
        else
        {
            TextLog_Puts(full_log, "[**]\n");
        }
    }

    if(p && IPH_IS_VALID(p))
    {
        LogPriorityData(full_log, event, true);
    }

    DEBUG_WRAP(DebugMessage(DEBUG_LOG, "Logging Alert data!\n"););

    LogTimeStamp(full_log, p);

    if(p && IPH_IS_VALID(p))
    {
        /* print the packet header to the alert file */

        if (ScOutputDataLink())
        {
            Log2ndHeader(full_log, p);
        }

        LogIPHeader(full_log, p);

        /* if this isn't a fragment, print the other header info */
        if(!p->frag_flag)
        {
            switch(GET_IPH_PROTO(p))
            {
                case IPPROTO_TCP:
                   LogTCPHeader(full_log, p);
                    break;

                case IPPROTO_UDP:
                   LogUDPHeader(full_log, p);
                    break;

                case IPPROTO_ICMP:
                   LogICMPHeader(full_log, p);
                    break;

                default:
                    break;
            }
        }
        LogXrefs(full_log, event, 1);

        TextLog_Putc(full_log, '\n');
    } /* End of if(p) */
    else
    {
        TextLog_Puts(full_log, "\n\n");
    }
    TextLog_Flush(full_log);
}

//-------------------------------------------------------------------------
// api stuff
//-------------------------------------------------------------------------

static Module* mod_ctor()
{ return new FullModule; }

static void mod_dtor(Module* m)
{ delete m; }

static Logger* full_ctor(SnortConfig*, Module* mod)
{ return new FullLogger((FullModule*)mod); }

static void full_dtor(Logger* p)
{ delete p; }

static LogApi full_api
{
    {
        PT_LOGGER,
        "alert_full",
        LOGAPI_PLUGIN_V0,
        0,
        mod_ctor,
        mod_dtor
    },
    OUTPUT_TYPE_FLAG__ALERT,
    full_ctor,
    full_dtor
};

#ifdef BUILDING_SO
SO_PUBLIC const BaseApi* snort_plugins[] =
{
    &full_api.base,
    nullptr
};
#else
const BaseApi* alert_full = &full_api.base;
#endif

