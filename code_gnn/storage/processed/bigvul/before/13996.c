gsicc_set_device_profile_colorants(gx_device *dev, char *name_str)
{
    int code;
    cmm_dev_profile_t *profile_struct;
    gsicc_colorname_t *name_entry;
    gsicc_colorname_t **curr_entry;
    gs_memory_t *mem;
    char *temp_ptr, *last = NULL;
    int done;
    gsicc_namelist_t *spot_names;
    char *pch;
    int str_len;
    int k;
    bool free_str = false;

    code = dev_proc(dev, get_profile)((gx_device *)dev, &profile_struct);
    if (profile_struct != NULL) {
        int count = 0;

        if (name_str == NULL) {
             
            int total_len;
            int kk;
            int num_comps = profile_struct->device_profile[0]->num_comps;
            char temp_str[DEFAULT_ICC_COLORANT_LENGTH+2];

             
            if (profile_struct->spotnames != NULL)
                return 0;

            free_str = true;
             
            total_len = ((DEFAULT_ICC_COLORANT_LENGTH + 1) * (num_comps-4)) +
                        DEFAULT_ICC_PROCESS_LENGTH - 1;   
            name_str = (char*) gs_alloc_bytes(dev->memory, total_len+1,
                                               "gsicc_set_device_profile_colorants");
            if (name_str == NULL)
                return gs_throw(gs_error_VMerror, "Insufficient memory for colorant name");
            gs_sprintf(name_str, DEFAULT_ICC_PROCESS);
            for (kk = 0; kk < num_comps-5; kk++) {
                gs_sprintf(temp_str,"ICC_COLOR_%d,",kk);
                strcat(name_str,temp_str);
            }
             
            gs_sprintf(temp_str,"ICC_COLOR_%d",kk);
            strcat(name_str,temp_str);
        }
        str_len = strlen(name_str);
        if (profile_struct->spotnames != NULL &&
            profile_struct->spotnames->name_str != NULL &&
            strlen(profile_struct->spotnames->name_str) == str_len) {
             
            if (strncmp(name_str, profile_struct->spotnames->name_str, str_len) == 0) {
                if (free_str)
                    gs_free_object(dev->memory, name_str,
                                            "gsicc_set_device_profile_colorants");
                return 0;
            }
        }
        mem = dev->memory->non_gc_memory;
         
        if (profile_struct->spotnames != NULL) {
             
            gsicc_free_spotnames(profile_struct->spotnames, mem);
             
            gs_free_object(mem, profile_struct->spotnames,
                           "gsicc_set_device_profile_colorants");
        }
         
        spot_names = gsicc_new_namelist(mem);
        profile_struct->spotnames = spot_names;
        spot_names->name_str = (char*) gs_alloc_bytes(mem, str_len+1,
                                               "gsicc_set_device_profile_colorants");
        if (spot_names->name_str == NULL)
            return gs_throw(gs_error_VMerror, "Insufficient memory for spot name");
        memcpy(spot_names->name_str, name_str, strlen(name_str));
        spot_names->name_str[str_len] = 0;
        curr_entry = &(spot_names->head);
          
        pch = gs_strtok(name_str, ",", &last);
        count = 0;
        while (pch != NULL) {
            temp_ptr = pch;
            done = 0;
             
            while (!done) {
                if (*temp_ptr == 0x20) {
                    temp_ptr++;
                } else {
                    done = 1;
                }
            }
             
            name_entry = gsicc_new_colorname(mem);
             
            *curr_entry = name_entry;
            name_entry->length = strlen(temp_ptr);
            name_entry->name = (char *) gs_alloc_bytes(mem, name_entry->length,
                                        "gsicc_set_device_profile_colorants");
            if (spot_names->name_str == NULL)
                return gs_throw(gs_error_VMerror, "Insufficient memory for spot name");
            memcpy(name_entry->name, temp_ptr, name_entry->length);
             
            curr_entry = &((*curr_entry)->next);
            count += 1;
            pch = gs_strtok(NULL, ",", &last);
        }
        spot_names->count = count;
         
        spot_names->color_map =
            (gs_devicen_color_map*) gs_alloc_bytes(mem,
                                                   sizeof(gs_devicen_color_map),
                                                   "gsicc_set_device_profile_colorants");
        if (spot_names->color_map == NULL)
            return gs_throw(gs_error_VMerror, "Insufficient memory for spot color map");
        spot_names->color_map->num_colorants = count;
        spot_names->color_map->num_components = count;

        name_entry = spot_names->head;
        for (k = 0; k < count; k++) {
            int colorant_number = (*dev_proc(dev, get_color_comp_index))
                    (dev, (const char *)name_entry->name, name_entry->length,
                     SEPARATION_NAME);
            name_entry = name_entry->next;
            spot_names->color_map->color_map[k] = colorant_number;
        }
         
        spot_names->equiv_cmyk_set = false;
        if (free_str)
            gs_free_object(dev->memory, name_str,
                           "gsicc_set_device_profile_colorants");
    }
    return code;
}
