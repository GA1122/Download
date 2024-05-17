print_cluster_tickets(pe_working_set_t * data_set)
{
    xmlNode *cib_constraints = get_object_root(XML_CIB_TAG_CONSTRAINTS, data_set->input);

     
    unpack_constraints(cib_constraints, data_set);

    print_as("\nTickets:\n");
    g_hash_table_foreach(data_set->tickets, print_ticket, NULL);

    return;
}