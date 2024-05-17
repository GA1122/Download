    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule = ofm->temp_rule;

     
    ofproto_rule_ref(rule);
    enum ofperr error = ofproto_flow_mod_start(rule->ofproto, ofm);
    ofm->temp_rule = rule;

    return error;
}
