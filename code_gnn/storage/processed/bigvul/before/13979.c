static void gsicc_manager_free_contents(gsicc_manager_t *icc_manager,
                                  client_name_t cname)
{
    int k;
    gsicc_devicen_entry_t *device_n, *device_n_next;

    rc_decrement(icc_manager->default_cmyk, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->default_gray, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->default_rgb, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->device_named, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->lab_profile, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->graytok_profile, "gsicc_manager_free_contents");
    rc_decrement(icc_manager->srcgtag_profile, "gsicc_manager_free_contents");

     
    if ( icc_manager->device_n != NULL) {
        device_n = icc_manager->device_n->head;
        for ( k = 0; k < icc_manager->device_n->count; k++) {
            rc_decrement(device_n->iccprofile, "gsicc_manager_free_contents");
            device_n_next = device_n->next;
            gs_free_object(icc_manager->memory, device_n, "gsicc_manager_free_contents");
            device_n = device_n_next;
        }
        gs_free_object(icc_manager->memory, icc_manager->device_n,
                       "gsicc_manager_free_contents");
    }

     
    if (icc_manager->smask_profiles != NULL) {
        gs_free_object(icc_manager->smask_profiles->memory, icc_manager->smask_profiles, "gsicc_manager_free_contents");
        icc_manager->smask_profiles = NULL;
    }
}
