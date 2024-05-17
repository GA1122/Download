    OVS_REQUIRES(ofproto_mutex)
{
    struct oftable *table;
    size_t n_readonly = 0;
    enum ofperr error = 0;

    rule_collection_init(rules);

    if (!check_table_id(ofproto, criteria->table_id)) {
        error = OFPERR_OFPBRC_BAD_TABLE_ID;
        goto exit;
    }

    if (criteria->cookie_mask == OVS_BE64_MAX) {
        struct rule *rule;

        HINDEX_FOR_EACH_WITH_HASH (rule, cookie_node,
                                   hash_cookie(criteria->cookie),
                                   &ofproto->cookies) {
            if (cls_rule_equal(&rule->cr, &criteria->cr)) {
                collect_rule(rule, criteria, rules, &n_readonly);
            }
        }
    } else {
        FOR_EACH_MATCHING_TABLE (table, criteria->table_id, ofproto) {
            struct rule *rule;

            rule = rule_from_cls_rule(classifier_find_rule_exactly(
                                          &table->cls, &criteria->cr,
                                          criteria->version));
            if (rule) {
                collect_rule(rule, criteria, rules, &n_readonly);
            }
        }
    }

exit:
    if (!error && !rule_collection_n(rules) && n_readonly) {
         
        error = OFPERR_OFPBRC_EPERM;
    }
    if (error) {
        rule_collection_destroy(rules);
    }
    return error;
}
