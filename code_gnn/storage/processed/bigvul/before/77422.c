replace_rule_revert(struct ofproto *ofproto,
                    struct rule *old_rule, struct rule *new_rule)
{
    struct oftable *table = &ofproto->tables[new_rule->table_id];

    if (old_rule) {
        if (old_rule->removed_reason == OFPRR_EVICTION) {
             
            eviction_group_add_rule(old_rule);
        }

         
        ofproto_rule_insert__(ofproto, old_rule);

         
        cls_rule_restore_visibility(&old_rule->cr);
    } else {
         
        table->n_flows--;
    }

     
    if (!classifier_remove(&table->cls, &new_rule->cr)) {
        OVS_NOT_REACHED();
    }
    ofproto_rule_remove__(ofproto, new_rule);
    ofproto_rule_unref(new_rule);
}
