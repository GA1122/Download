print_rsc_summary(pe_working_set_t * data_set, node_t * node, resource_t * rsc, gboolean all)
{
    gboolean printed = FALSE;
    time_t last_failure = 0;

    char *fail_attr = crm_concat("fail-count", rsc->id, '-');
    const char *value = g_hash_table_lookup(node->details->attrs, fail_attr);

    int failcount = char2score(value);   

    get_failcount(node, rsc, (int *)&last_failure, data_set);
    free(fail_attr);

    if (all || failcount || last_failure > 0) {
        printed = TRUE;
        print_as("   %s: migration-threshold=%d", rsc->id, rsc->migration_threshold);
    }

    if (failcount > 0) {
        printed = TRUE;
        print_as(" fail-count=%d", failcount);
    }

    if (last_failure > 0) {
        printed = TRUE;
        print_as(" last-failure=");
        print_date(last_failure);
    }

    if (printed) {
        print_as("\n");
    }
}