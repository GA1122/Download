    OVS_EXCLUDED(ofproto_mutex)
{
     
    ovs_mutex_lock(&ofproto_mutex);

    if (rule->state == RULE_INSERTED) {
         
        ovs_assert(cls_rule_visible_in_version(&rule->cr, OVS_VERSION_MAX));

        if (!classifier_remove(&rule->ofproto->tables[rule->table_id].cls,
                               &rule->cr)) {
            OVS_NOT_REACHED();
        }
        ofproto_rule_remove__(rule->ofproto, rule);
        if (ofproto->ofproto_class->rule_delete) {
            ofproto->ofproto_class->rule_delete(rule);
        }

         
        ofproto_rule_unref(rule);
    }
    ovs_mutex_unlock(&ofproto_mutex);
}
