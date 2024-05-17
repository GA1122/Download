    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule;

    if ((table->eviction != 0) == (eviction != 0)
        && n_fields == table->n_eviction_fields
        && (!n_fields
            || !memcmp(fields, table->eviction_fields,
                       n_fields * sizeof *fields))) {
         
        table->eviction = eviction;
        return;
    }

     
    struct eviction_group *evg, *next;
    HMAP_FOR_EACH_SAFE (evg, next, id_node, &table->eviction_groups_by_id) {
        eviction_group_destroy(table, evg);
    }
    hmap_destroy(&table->eviction_groups_by_id);
    hmap_init(&table->eviction_groups_by_id);
    heap_destroy(&table->eviction_groups_by_size);
    heap_init(&table->eviction_groups_by_size);

     
    struct mf_subfield *old_fields = table->eviction_fields;
    table->n_eviction_fields = n_fields;
    table->eviction_fields = (fields
                              ? xmemdup(fields, n_fields * sizeof *fields)
                              : NULL);
    free(old_fields);

     
    table->eviction = eviction;
    if (table->eviction) {
        table->eviction_group_id_basis = random_uint32();
        CLS_FOR_EACH (rule, cr, &table->cls) {
            eviction_group_add_rule(rule);
        }
    }
}
