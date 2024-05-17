    OVS_REQUIRES(ofproto_mutex)
{
    struct classifier *cls = &ofproto->tables[0].cls;
    struct rule *rule;

     
    rule = rule_from_cls_rule(classifier_find_match_exactly(
                                  cls, target, priority, OVS_VERSION_MAX));
    if (!rule) {
        return;
    }

    struct rule_collection rules;

    rule_collection_init(&rules);
    rule_collection_add(&rules, rule);
    delete_flows__(&rules, OFPRR_DELETE, NULL);
    rule_collection_destroy(&rules);
}
