    OVS_EXCLUDED(ofproto_mutex)
{
    const struct rule *rule;
    bool must_add;

     
    rule = rule_from_cls_rule(classifier_find_match_exactly(
                                  &ofproto->tables[0].cls, match, priority,
                                  OVS_VERSION_MAX));
    if (rule) {
        const struct rule_actions *actions = rule_get_actions(rule);
        must_add = !ofpacts_equal(actions->ofpacts, actions->ofpacts_len,
                                  ofpacts, ofpacts_len);
    } else {
        must_add = true;
    }

     
    if (must_add) {
        simple_flow_mod(ofproto, match, priority, ofpacts, ofpacts_len,
                        OFPFC_MODIFY_STRICT);
    }
}
