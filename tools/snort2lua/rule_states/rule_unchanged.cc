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
// rule_content.cc author Josh Rosenbaum <jrosenba@cisco.com>

#include <sstream>
#include <vector>

#include "conversion_state.h"
#include "utils/converter.h"
#include "rule_states/rule_api.h"
#include "utils/snort2lua_util.h"

namespace rules
{

/*
 *  To use this template, declared an 'unchanged_rule_ctor<rule_name>'
 *  in the ConvertMap struct.  Everything else will be taken care of and
 *  all of the data between two semicolons will be placed in the new rule AS IS!!
 *  Examples are below the line marked 'FINISHED TEMPLATES'.
 */

template<const std::string* rule_name, bool has_suboptions>
class UnchangedRuleOption : public ConversionState
{
public:
    UnchangedRuleOption( Converter* cv, LuaData* ld)
        :   ConversionState(cv, ld)
    { };
    virtual ~UnchangedRuleOption() {};
    
    virtual bool convert(std::istringstream& stream)
    {
        bool retval;

        if (has_suboptions)
        {
            std::string val = util::get_rule_option_args(stream);
            retval = ld->add_rule_option(*rule_name, val);
        }
        else
        {
            retval = ld->add_rule_option(*rule_name);
        }

        return set_next_rule_state(stream) && retval;
    }
};


template<const std::string *rule_name,  bool has_suboptions = true>
static ConversionState* unchanged_rule_ctor(Converter* cv, LuaData* ld)
{
    return new UnchangedRuleOption<rule_name, has_suboptions>(cv, ld);
}

/****************************************
 *******   FINISHED TEMPLATES ***********
 ****************************************/

/************************************
 **********  M S G ******************
 ************************************/

static const std::string msg = "msg";
static const ConvertMap rule_msg =
{
    msg,
    unchanged_rule_ctor<&msg>,
};

const ConvertMap* msg_map = &rule_msg;

/************************************
 **********  G I D ******************
 ************************************/

static const std::string gid = "gid";
static const ConvertMap rule_gid =
{
    gid,
    unchanged_rule_ctor<&gid>,
};

const ConvertMap* gid_map = &rule_gid;

/************************************
 **********  S I D  *****************
 ************************************/

static const std::string sid = "sid";
static const ConvertMap rule_sid =
{
    sid,
    unchanged_rule_ctor<&sid>,
};

const ConvertMap* sid_map = &rule_sid;

/************************************
 **********  R E V  *****************
 ************************************/

static const std::string rev = "rev";
static const ConvertMap rule_rev =
{
    rev,
    unchanged_rule_ctor<&rev>,
};

const ConvertMap* rev_map = &rule_rev;

/************************************
 ********  REFERENCE  ***************
 ************************************/

static const std::string reference = "reference";
static const ConvertMap rule_reference =
{
    reference,
    unchanged_rule_ctor<&reference>,
};

const ConvertMap* reference_map = &rule_reference;

/************************************
 **********  CLASSTYPE  *************
 ************************************/

static const std::string classtype = "classtype";
static const ConvertMap rule_classtype =
{
    classtype,
    unchanged_rule_ctor<&classtype>,
};

const ConvertMap* classtype_map = &rule_classtype;

/************************************
 **********  F L O W  ***************
 ************************************/

static const std::string flow = "flow";
static const ConvertMap rule_flow =
{
    flow,
    unchanged_rule_ctor<&flow>,
};

const ConvertMap* flow_map = &rule_flow;

/************************************
 **********  FLOWBITS  **************
 ************************************/

static const std::string flowbits = "flowbits";
static const ConvertMap rule_flowbits =
{
    flowbits,
    unchanged_rule_ctor<&flowbits>,
};

const ConvertMap* flowbits_map = &rule_flowbits;

/************************************
 *********  D S I Z E  **************
 ************************************/

static const std::string dsize = "dsize";
static const ConvertMap rule_dsize =
{
    dsize,
    unchanged_rule_ctor<&dsize>,
};

const ConvertMap* dsize_map = &rule_dsize;

/************************************
 *********  D S I Z E  **************
 ************************************/

static const std::string fragbits = "fragbits";
static const ConvertMap rule_fragbits =
{
    fragbits,
    unchanged_rule_ctor<&fragbits>,
};

const ConvertMap* fragbits_map = &rule_fragbits;

/************************************
 *********  F L A G S  **************
 ************************************/


static const std::string flags = "flags";
static const ConvertMap rule_flags =
{
    flags,
    unchanged_rule_ctor<&flags>,
};

const ConvertMap* flags_map = &rule_flags;

/************************************
 *********  FRAGOFFSET **************
 ************************************/


static const std::string fragoffset = "fragoffset";
static const ConvertMap rule_fragoffset =
{
    fragoffset,
    unchanged_rule_ctor<&fragoffset>,
};

const ConvertMap* fragoffset_map = &rule_fragoffset;

/************************************
 ************* T T L ****************
 ************************************/


static const std::string ttl = "ttl";
static const ConvertMap rule_ttl =
{
    ttl,
    unchanged_rule_ctor<&ttl>,
};

const ConvertMap* ttl_map = &rule_ttl;


/************************************
 ************* T O S ****************
 ************************************/

static const std::string tos = "tos";
static const ConvertMap rule_tos =
{
    tos,
    unchanged_rule_ctor<&tos>,
};

const ConvertMap* tos_map = &rule_tos;

/************************************
 *************  I D  ****************
 ************************************/

static const std::string id = "id";
static const ConvertMap rule_id =
{
    id,
    unchanged_rule_ctor<&id>,
};

const ConvertMap* id_map = &rule_id;

/************************************
 ***********  IPOPTS  ***************
 ************************************/

static const std::string ipopts = "ipopts";
static const ConvertMap rule_ipopts =
{
    ipopts,
    unchanged_rule_ctor<&ipopts>,
};

const ConvertMap* ipopts_map = &rule_ipopts;

/************************************
 ************  S E Q  ***************
 ************************************/

static const std::string seq = "seq";
static const ConvertMap rule_seq =
{
    seq,
    unchanged_rule_ctor<&seq>,
};

const ConvertMap* seq_map = &rule_seq;

/************************************
 ************  A C K  ***************
 ************************************/

static const std::string ack = "ack";
static const ConvertMap rule_ack =
{
    ack,
    unchanged_rule_ctor<&ack>,
};

const ConvertMap* ack_map = &rule_ack;

/************************************
 ***********  WINDOW  ***************
 ************************************/

static const std::string window = "window";
static const ConvertMap rule_window =
{
    window,
    unchanged_rule_ctor<&window>,
};

const ConvertMap* window_map = &rule_window;

/************************************
 ************  ITYPE  ***************
 ************************************/

static const std::string itype = "itype";
static const ConvertMap rule_itype =
{
    itype,
    unchanged_rule_ctor<&itype>,
};

const ConvertMap* itype_map = &rule_itype;

/************************************
 ************  ICODE  ***************
 ************************************/

static const std::string icode = "icode";
static const ConvertMap rule_icode =
{
    icode,
    unchanged_rule_ctor<&icode>,
};

const ConvertMap* icode_map = &rule_icode;

/************************************
 ************ ICMP_ID ***************
 ************************************/


static const std::string icmp_id = "icmp_id";
static const ConvertMap rule_icmp_id =
{
    icmp_id,
    unchanged_rule_ctor<&icmp_id>,
};

const ConvertMap* icmp_id_map = &rule_icmp_id;

/************************************
 ***********  ICMP_SEQ  *************
 ************************************/

static const std::string icmp_seq = "icmp_seq";
static const ConvertMap rule_icmp_seq =
{
    icmp_seq,
    unchanged_rule_ctor<&icmp_seq>,
};

const ConvertMap* icmp_seq_map = &rule_icmp_seq;

/************************************
 *************  R P C  **************
 ************************************/

static const std::string rpc = "rpc";
static const ConvertMap rule_rpc =
{
    rpc,
    unchanged_rule_ctor<&rpc>,
};

const ConvertMap* rpc_map = &rule_rpc;

/************************************
 ***********  IP_PROTO  *************
 ************************************/

static const std::string ip_proto = "ip_proto";
static const ConvertMap rule_ip_proto =
{
    ip_proto,
    unchanged_rule_ctor<&ip_proto>,
};

const ConvertMap* ip_proto_map = &rule_ip_proto;

/************************************
 ************  SAME_IP  *************
 ************************************/

static const std::string sameip = "sameip";
static const ConvertMap rule_sameip =
{
    sameip,
    unchanged_rule_ctor<&sameip>,
};

const ConvertMap* sameip_map = &rule_sameip;

/************************************
 ******  STREAM_RESSAMBLE  **********
 ************************************/

static const std::string stream_reassemble = "stream_reassemble";
static const ConvertMap rule_stream_reassemble =
{
    stream_reassemble,
    unchanged_rule_ctor<&stream_reassemble>,
};

const ConvertMap* stream_reassemble_map = &rule_stream_reassemble;

/************************************
 *********  STREAM_SIZE  ************
 ************************************/

static const std::string stream_size = "stream_size";
static const ConvertMap rule_stream_size =
{
    stream_size,
    unchanged_rule_ctor<&stream_size>,
};

const ConvertMap* stream_size_map = &rule_stream_size;

/************************************
 ************  LOG TO  **************
 ************************************/

static const std::string logto = "logto";
static const ConvertMap rule_logto =
{
    logto,
    unchanged_rule_ctor<&logto>,
};

const ConvertMap* logto_map = &rule_logto;

/************************************
 *************  SESSION  ************
 ************************************/

static const std::string session = "session";
static const ConvertMap rule_session =
{
    session,
    unchanged_rule_ctor<&session>,
};

const ConvertMap* session_map = &rule_session;

/************************************
 *************** RESP  **************
 ************************************/

static const std::string resp = "resp";
static const ConvertMap rule_resp =
{
    resp,
    unchanged_rule_ctor<&resp>,
};

const ConvertMap* resp_map = &rule_resp;

/************************************
 **************  REACT  *************
 ************************************/

static const std::string react = "react";
static const ConvertMap rule_react =
{
    react,
    unchanged_rule_ctor<&react>,
};

const ConvertMap* react_map = &rule_react;

/************************************
 ***************  TAG  **************
 ************************************/

static const std::string tag = "tag";
static const ConvertMap rule_tag =
{
    tag,
    unchanged_rule_ctor<&tag>,
};

const ConvertMap* tag_map = &rule_tag;

/************************************
 ************* REPLACE  *************
 ************************************/

static const std::string replace = "replace";
static const ConvertMap rule_replace =
{
    replace,
    unchanged_rule_ctor<&replace>,
};

const ConvertMap* replace_map = &rule_replace;

/************************************
 ******* DETECTION_FILETER  *********
 ************************************/

static const std::string detection_filter = "detection_filter";
static const ConvertMap rule_detection_filter =
{
    detection_filter,
    unchanged_rule_ctor<&detection_filter>,
};

const ConvertMap* detection_filter_map = &rule_detection_filter;

/************************************
 ***********  THRESHOLD  ************
 ************************************/

static const std::string threshold = "threshold";
static const ConvertMap rule_threshold =
{
    threshold,
    unchanged_rule_ctor<&threshold>,
};

const ConvertMap* threshold_map = &rule_threshold;

/************************************
 ***********  BYTE_TEST  ************
 ************************************/

static const std::string byte_test = "byte_test";
static const ConvertMap rule_byte_test =
{
    byte_test,
    unchanged_rule_ctor<&byte_test>,
};

const ConvertMap* byte_test_map = &rule_byte_test;


/************************************
 ***********  BYTE_JUMP  ************
 ************************************/

static const std::string byte_jump = "byte_jump";
static const ConvertMap rule_byte_jump =
{
    byte_jump,
    unchanged_rule_ctor<&byte_jump>,
};

const ConvertMap* byte_jump_map = &rule_byte_jump;

/************************************
 **********  BYTE_EXTRACT  **********
 ************************************/

static const std::string byte_extract = "byte_extract";
static const ConvertMap rule_byte_extract =
{
    byte_extract,
    unchanged_rule_ctor<&byte_extract>,
};

const ConvertMap* byte_extract_map = &rule_byte_extract;


/************************************
 ************  PKT_DATA  ************
 ************************************/

static const std::string pkt_data = "pkt_data";
static const ConvertMap rule_pkt_data =
{
    pkt_data,
    unchanged_rule_ctor<&pkt_data, false>,
};

const ConvertMap* pkt_data_map = &rule_pkt_data;

/************************************
 *************  CVS  ****************
 ************************************/

static const std::string cvs = "cvs";
static const ConvertMap rule_cvs =
{
    cvs,
    unchanged_rule_ctor<&cvs>,
};

const ConvertMap* cvs_map = &rule_cvs;

/************************************
 *********  BASE64_DATA  ************
 ************************************/

static const std::string base64_data = "base64_data";
static const ConvertMap rule_base64_data =
{
    base64_data,
    unchanged_rule_ctor<&base64_data, false>,
};

const ConvertMap* base64_data_map = &rule_base64_data;

/************************************
 *********** ISDATAAT  **************
 ************************************/

static const std::string isdataat = "isdataat";
static const ConvertMap rule_isdataat =
{
    isdataat,
    unchanged_rule_ctor<&isdataat>,
};

const ConvertMap* isdataat_map = &rule_isdataat;

/************************************
 *************  ASN1  ***************
 ************************************/

static const std::string asn1 = "asn1";
static const ConvertMap rule_asn1 =
{
    asn1,
    unchanged_rule_ctor<&asn1>,
};

const ConvertMap* asn1_map = &rule_asn1;

/************************************
 ***********  PRIORITY  *************
 ************************************/

static const std::string priority = "priority";
static const ConvertMap rule_priority =
{
    priority,
    unchanged_rule_ctor<&priority>,
};

const ConvertMap* priority_map = &rule_priority;



#if 0

"activates"
"activated_by"
"count"




THRESHOLD

Option
Description
type limit|threshold|both
type limit alerts on the 1st m events during the time interval, then ignores events for the rest of the time interval. Type threshold alerts every m times we see this event during the time interval. Type both alerts once per time interval after seeing m occurrences of the event, then ignores any additional events during the time interval.
track by src|by dst
￼￼
rate is tracked either by source IP address, or destination IP address. This means count is maintained for each unique source IP addresses, or for each unique destination IP addresses. Ports or anything else are not tracked.
count c
number of rule matching in s seconds that will cause event filter limit to be exceeded. c must be nonzero value.
￼
seconds s

#endif

} // namespace rule
