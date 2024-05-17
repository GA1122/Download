    OVS_REQUIRES(ofproto_mutex)
{
    struct eviction_group *evg;

    *rulep = NULL;
    if (!table->eviction) {
        return false;
    }

     
    HEAP_FOR_EACH (evg, size_node, &table->eviction_groups_by_size) {
        struct rule *rule;

        HEAP_FOR_EACH (rule, evg_node, &evg->rules) {
            *rulep = rule;
            return true;
        }
    }

    return false;
}
