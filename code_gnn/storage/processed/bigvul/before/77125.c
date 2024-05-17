    OVS_EXCLUDED(ofproto_mutex)
{
     
    if (fm->command != OFPFC_MODIFY_STRICT || fm->table_id == OFPTT_ALL
        || fm->flags & OFPUTIL_FF_RESET_COUNTS
        || fm->buffer_id != UINT32_MAX) {
        return OFPERR_OFPFMFC_UNKNOWN;
    }

     
    struct oftable *table = &ofproto->tables[fm->table_id];
    struct rule *rule;

    rule = rule_from_cls_rule(classifier_find_match_exactly(
                                  &table->cls, &fm->match, fm->priority,
                                  OVS_VERSION_MAX));
    if (rule) {
         
        const struct rule_actions *actions;

        ovs_mutex_lock(&rule->mutex);
        actions = rule_get_actions(rule);
        if (rule->idle_timeout == fm->idle_timeout
            && rule->hard_timeout == fm->hard_timeout
            && rule->importance == fm->importance
            && rule->flags == (fm->flags & OFPUTIL_FF_STATE)
            && (!fm->modify_cookie || (fm->new_cookie == rule->flow_cookie))
            && ofpacts_equal(fm->ofpacts, fm->ofpacts_len,
                             actions->ofpacts, actions->ofpacts_len)) {
             
            ovs_mutex_unlock(&rule->mutex);
            if (!ofproto_rule_try_ref(rule)) {
                rule = NULL;  
            }
        } else {
            ovs_mutex_unlock(&rule->mutex);
            rule = NULL;
        }
    }

    return ofproto_flow_mod_init(ofproto, ofm, fm, rule);
}
