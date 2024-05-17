ofproto_flow_mod_learn_refresh(struct ofproto_flow_mod *ofm)
{
    enum ofperr error = 0;

     
    struct rule *rule = ofm->temp_rule;
    if (!rule) {
        return OFPERR_OFPFMFC_UNKNOWN;
    }

     
    if (rule->state == RULE_REMOVED) {
        struct cls_rule cr;

        cls_rule_clone(&cr, &rule->cr);
        ovs_mutex_lock(&rule->mutex);
        error = ofproto_rule_create(rule->ofproto, &cr, rule->table_id,
                                    rule->flow_cookie,
                                    rule->idle_timeout,
                                    rule->hard_timeout, rule->flags,
                                    rule->importance,
                                    rule->actions->ofpacts,
                                    rule->actions->ofpacts_len,
                                    rule->match_tlv_bitmap,
                                    rule->ofpacts_tlv_bitmap,
                                    &ofm->temp_rule);
        ovs_mutex_unlock(&rule->mutex);
        if (!error) {
            ofproto_rule_unref(rule);    
        }
    } else {
         
        ovs_mutex_lock(&rule->mutex);
        rule->modified = time_msec();
        ovs_mutex_unlock(&rule->mutex);
    }
    return error;
}
