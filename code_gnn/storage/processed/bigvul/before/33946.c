print_attr_msg(node_t * node, GListPtr rsc_list, const char *attrname, const char *attrvalue)
{
    GListPtr gIter = NULL;

    for (gIter = rsc_list; gIter != NULL; gIter = gIter->next) {
        resource_t *rsc = (resource_t *) gIter->data;
        const char *type = g_hash_table_lookup(rsc->meta, "type");

        if (rsc->children != NULL) {
            print_attr_msg(node, rsc->children, attrname, attrvalue);
        }

        if (safe_str_eq(type, "ping") || safe_str_eq(type, "pingd")) {
            const char *name = "pingd";
            const char *multiplier = NULL;
            char **host_list = NULL;
            int host_list_num = 0;
            int expected_score = 0;

            if (g_hash_table_lookup(rsc->meta, "name") != NULL) {
                name = g_hash_table_lookup(rsc->meta, "name");
            }

             
            if (safe_str_eq(name, attrname)) {
                int value = crm_parse_int(attrvalue, "0");

                multiplier = g_hash_table_lookup(rsc->meta, "multiplier");
                host_list = g_strsplit(g_hash_table_lookup(rsc->meta, "host_list"), " ", 0);
                host_list_num = g_strv_length(host_list);
                g_strfreev(host_list);
                 
                expected_score = host_list_num * crm_parse_int(multiplier, "1");

                 
                if (value <= 0) {
                    print_as("\t: Connectivity is lost");
                } else if (value < expected_score) {
                    print_as("\t: Connectivity is degraded (Expected=%d)", expected_score);
                }
            }
        }
    }
}