gsicc_new_devicen(gsicc_manager_t *icc_manager)
{
 
    gsicc_devicen_entry_t *device_n_entry =
        gs_alloc_struct(icc_manager->memory, gsicc_devicen_entry_t,
                &st_gsicc_devicen_entry, "gsicc_new_devicen");
    if (device_n_entry == NULL)
        return gs_throw(gs_error_VMerror, "insufficient memory to allocate device n profile");
    device_n_entry->next = NULL;
    device_n_entry->iccprofile = NULL;
 
    if ( icc_manager->device_n == NULL ) {
         
        icc_manager->device_n = gs_alloc_struct(icc_manager->memory,
            gsicc_devicen_t, &st_gsicc_devicen, "gsicc_new_devicen");

        if (icc_manager->device_n == NULL)
            return gs_throw(gs_error_VMerror, "insufficient memory to allocate device n profile");

        icc_manager->device_n->head = device_n_entry;
        icc_manager->device_n->final = device_n_entry;
        icc_manager->device_n->count = 1;
        return 0;
    } else {
         
        icc_manager->device_n->final->next = device_n_entry;
        icc_manager->device_n->final = device_n_entry;
        icc_manager->device_n->count++;
        return 0;
    }
}
