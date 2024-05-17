gsicc_search_icc_table(clist_icctable_t *icc_table, int64_t icc_hashcode, int *size)
{
    int tablesize = icc_table->tablesize, k;
    clist_icctable_entry_t *curr_entry;

    curr_entry = icc_table->head;
    for (k = 0; k < tablesize; k++ ) {
        if ( curr_entry->serial_data.hashcode == icc_hashcode ) {
            *size = curr_entry->serial_data.size;
            return curr_entry->serial_data.file_position;
        }
        curr_entry = curr_entry->next;
    }

     
    *size = 0;
    return -1;
}
