    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule;

    RULE_COLLECTION_FOR_EACH (rule, rules) {
        struct oftable *table = &ofproto->tables[rule->table_id];

         
        ofproto_rule_insert__(ofproto, rule);

         
        table->n_flows++;

         
        cls_rule_restore_visibility(&rule->cr);
    }
}
