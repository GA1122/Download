    OVS_REQUIRES(ofproto_mutex)
{
    struct ofproto *ofproto = rule->ofproto;
    struct oftable *table = &ofproto->tables[rule->table_id];
    bool has_timeout;

     
    has_timeout = rule->hard_timeout || rule->idle_timeout;

    if (table->eviction && has_timeout) {
        struct eviction_group *evg;

        evg = eviction_group_find(table, eviction_group_hash_rule(rule));

        rule->eviction_group = evg;
        heap_insert(&evg->rules, &rule->evg_node,
                    rule_eviction_priority(ofproto, rule));
        eviction_group_resized(table, evg);
    }
}
