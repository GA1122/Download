gsicc_finddevicen(const gs_color_space *pcs, gsicc_manager_t *icc_manager)
{
    int k,j,i;
    gsicc_devicen_entry_t *curr_entry;
    int num_comps;
    const gs_separation_name *names = pcs->params.device_n.names;
    unsigned char *pname;
    unsigned int name_size;
    gsicc_devicen_t *devicen_profiles = icc_manager->device_n;
    gsicc_colorname_t *icc_spot_entry;
    int match_count = 0;
    bool permute_needed = false;

    num_comps = gs_color_space_num_components(pcs);

     
    curr_entry = devicen_profiles->head;
    for ( k = 0; k < devicen_profiles->count; k++ ) {
        if (curr_entry->iccprofile->num_comps == num_comps ) {

             
            for ( j = 0; j < num_comps; j++) {
                 
                pcs->params.device_n.get_colorname_string(icc_manager->memory,
                                                    names[j], &pname, &name_size);
                 
                icc_spot_entry = curr_entry->iccprofile->spotnames->head;
                for ( i = 0; i < num_comps; i++) {
                    if( strncmp((const char *) pname,
                        icc_spot_entry->name, name_size) == 0 ) {
                         
                        match_count++;
                        curr_entry->iccprofile->devicen_permute[j] = i;
                        if ( j != i) {
                             
                            permute_needed = true;
                        }
                        break;
                    } else
                        icc_spot_entry = icc_spot_entry->next;
                }
                if (match_count < j+1)
                    return(NULL);
            }
            if ( match_count == num_comps) {
                 
                curr_entry->iccprofile->devicen_permute_needed = permute_needed;
                return(curr_entry->iccprofile);
            }
            match_count = 0;
        }
    }
    return NULL;
}
