    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule = rule_collection_rules(&ofm->new_rules)[0];

     
    if (rule->ofproto != orig_ofproto) {
        ofproto_bump_tables_version(rule->ofproto);
    }
    ofproto_flow_mod_finish(rule->ofproto, ofm, NULL);
    if (rule->ofproto != orig_ofproto) {
        ofmonitor_flush(rule->ofproto->connmgr);
    }
}
