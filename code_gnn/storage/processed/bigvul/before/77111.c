    OVS_EXCLUDED(ofproto_mutex)
{
    struct oftable *table;

     
    if (ofproto->ofproto_class->flush) {
        ofproto->ofproto_class->flush(ofproto);
    }

     

    ovs_mutex_lock(&ofproto_mutex);
    OFPROTO_FOR_EACH_TABLE (table, ofproto) {
        struct rule_collection rules;
        struct rule *rule;

        if (table->flags & OFTABLE_HIDDEN) {
            continue;
        }

        rule_collection_init(&rules);

        CLS_FOR_EACH (rule, cr, &table->cls) {
            rule_collection_add(&rules, rule);
        }
        delete_flows__(&rules, OFPRR_DELETE, NULL);
    }
     
    ovs_mutex_unlock(&ofproto_mutex);
}
