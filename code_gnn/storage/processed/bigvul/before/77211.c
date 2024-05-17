    OVS_REQUIRES(ofproto_mutex)
{
    ovs_assert(rule->state == RULE_INSERTED);

    cookies_remove(ofproto, rule);

    eviction_group_remove_rule(rule);
    if (!ovs_list_is_empty(&rule->expirable)) {
        ovs_list_remove(&rule->expirable);
    }
    if (!ovs_list_is_empty(&rule->meter_list_node)) {
        ovs_list_remove(&rule->meter_list_node);
        ovs_list_init(&rule->meter_list_node);
    }

     
    const struct rule_actions *actions = rule_get_actions(rule);

    if (actions->has_groups) {
        const struct ofpact_group *a;

        OFPACT_FOR_EACH_TYPE_FLATTENED(a, GROUP, actions->ofpacts,
                                        actions->ofpacts_len) {
            struct ofgroup *group;

            group = ofproto_group_lookup(ofproto, a->group_id, OVS_VERSION_MAX,
                                         false);
            ovs_assert(group);

             
            if (!group->being_deleted) {
                group_remove_rule(group, rule);
            }
        }
    }

    rule->state = RULE_REMOVED;
}
