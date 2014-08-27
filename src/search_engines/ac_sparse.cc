/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
**  Copyright (C) 2013-2013 Sourcefire, Inc.
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
**
*/

#include "acsmx.h"
#include "acsmx2.h"

#include "snort_debug.h"
#include "snort_types.h"
#include "util.h"
#include "profiler.h"
#include "snort.h"
#include "framework/mpse.h"

//-------------------------------------------------------------------------
// "ac_sparse"
//-------------------------------------------------------------------------

class AcsMpse : public Mpse
{
private:
    ACSM_STRUCT2* obj;

public:
    AcsMpse(
        SnortConfig*,
        bool use_gc,
        void (*user_free)(void*),
        void (*tree_free)(void**),
        void (*list_free)(void**))
    : Mpse("ac_sparse", use_gc)
    {
        obj = acsmNew2(user_free, tree_free, list_free);
        if ( obj ) acsmSelectFormat2(obj, ACF_SPARSE);
    };
    ~AcsMpse()
    { if (obj) acsmFree2(obj); };

    int add_pattern(
        SnortConfig*, void* P, int m,
        unsigned noCase, unsigned offset, unsigned depth,
        unsigned negative, void* ID, int IID)
    {
        return acsmAddPattern2(
            obj, (unsigned char *)P, m,
            noCase, offset, depth, negative, ID, IID );
    };

    int prep_patterns(
        SnortConfig* sc, mpse_build_f build_tree, mpse_negate_f neg_list)
    {
        return acsmCompile2WithSnortConf(sc, obj, build_tree, neg_list);
    };

    int _search(
        const unsigned char* T, int n, mpse_action_f action,
        void* data, int* current_state)
    {
        // FIXIT-H make sure DFA and NFA flavors aren't swapped
        return acsmSearchSparseDFA(
            obj, (unsigned char *)T, n, action, data, current_state);
    };

    int print_info()
    {
      return acsmPrintDetailInfo2(obj);
    };

    int get_pattern_count()
    {
        return acsmPatternCount2(obj);
    };
};

//-------------------------------------------------------------------------
// api
//-------------------------------------------------------------------------

static Mpse* acs_ctor(
    SnortConfig* sc,
    class Module*,
    bool use_gc,
    void (*user_free)(void*),
    void (*tree_free)(void**),
    void (*list_free)(void**))
{
    return new AcsMpse(sc, use_gc, user_free, tree_free, list_free);
}

static void acs_dtor(Mpse* p)
{
    delete p;
}

static void acs_init()
{
    acsmx2_init_xlatcase();
    acsm_init_summary();
}

static void acs_print()
{
    acsmPrintSummaryInfo2();
}

static const MpseApi acs_api =
{
    {
        PT_SEARCH_ENGINE,
        "ac_sparse",
        SEAPI_PLUGIN_V0,
        0,
        nullptr,
        nullptr
    },
    false,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    acs_ctor,
    acs_dtor,
    acs_init,
    acs_print,
};

const BaseApi* se_ac_sparse = &acs_api.base;

