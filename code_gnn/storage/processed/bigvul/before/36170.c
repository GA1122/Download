int mk_vhost_fdt_worker_init()
{
    int i;
    int j;
    struct host *h;
    struct mk_list *list;
    struct mk_list *head;
    struct vhost_fdt_host *fdt;
    struct vhost_fdt_hash_table *ht;
    struct vhost_fdt_hash_chain *hc;

    if (config->fdt == MK_FALSE) {
        return -1;
    }

     

     
    pthread_mutex_lock(&mk_vhost_fdt_mutex);

     
    list = mk_mem_malloc_z(sizeof(struct mk_list));
    mk_list_init(list);

    mk_list_foreach(head, &config->hosts) {
        h = mk_list_entry(head, struct host, _head);

        fdt = mk_mem_malloc(sizeof(struct vhost_fdt_host));
        fdt->host = h;

         
        for (i = 0; i < VHOST_FDT_HASHTABLE_SIZE; i++) {
            ht = &fdt->hash_table[i];
            ht->av_slots = VHOST_FDT_HASHTABLE_CHAINS;

             
            for (j = 0; j < VHOST_FDT_HASHTABLE_CHAINS; j++) {
                hc = &ht->chain[j];
                hc->fd      = -1;
                hc->hash    =  0;
                hc->readers =  0;
            }
        }
        mk_list_add(&fdt->_head, list);
    }

    mk_vhost_fdt_key = list;
    pthread_mutex_unlock(&mk_vhost_fdt_mutex);

    return 0;
}
