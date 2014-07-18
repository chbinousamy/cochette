/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * Copyright (C) 2002-2013 Sourcefire, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
// config_event_queue.cc author Josh Rosenbaum <jrosenba@cisco.com>

#include <sstream>
#include <vector>

#include "conversion_state.h"
#include "utils/converter.h"
#include "utils/snort2lua_util.h"

namespace config
{

namespace {

class EventQueue : public ConversionState
{
public:
    EventQueue(Converter* cv, LuaData* ld) : ConversionState(cv, ld) {};
    virtual ~EventQueue() {};
    virtual bool convert(std::istringstream& data_stream);
};

} // namespace

bool EventQueue::convert(std::istringstream& data_stream)
{
    std::string keyword;
    bool retval = true;

    ld->open_table("event_queue");

    while (util::get_string(data_stream, keyword, ", "))
    {
        bool tmpval = true;


        if (!keyword.compare("process_all_events"))
            tmpval = ld->add_option_to_table("process_all_events", true);

        else if (!keyword.compare("max_queue"))
        {
            std::string val;

            if(util::get_string(data_stream, val, ", "))
                tmpval = ld->add_option_to_table("max_queue", std::stoi(val));
            else
                tmpval = false;
        }

        else if (!keyword.compare("log"))
        {
            std::string val;

            if(util::get_string(data_stream, val, ", "))
                tmpval = ld->add_option_to_table("log", std::stoi(val));
            else
                tmpval = false;
        }

        else if (!keyword.compare("order_events"))
        {
            std::string val;
            if(util::get_string(data_stream, val, ", "))
                tmpval = ld->add_option_to_table("order_events", val);
            else
                tmpval = false;
        }

        else
            tmpval = false;

        if (retval && !tmpval)
            retval = false;
    }

    return retval;
}

/**************************
 *******  A P I ***********
 **************************/

static ConversionState* ctor(Converter* cv, LuaData* ld)
{
    return new EventQueue(cv, ld);
}

static const ConvertMap event_queue_api =
{
    "event_queue",
    ctor,
};

const ConvertMap* event_queue_map = &event_queue_api;

} // namespace config