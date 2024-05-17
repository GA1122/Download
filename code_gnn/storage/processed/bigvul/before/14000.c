gsicc_set_profile(gsicc_manager_t *icc_manager, const char* pname, int namelen,
                  gsicc_profile_t defaulttype)
{
    cmm_profile_t *icc_profile;
    cmm_profile_t **manager_default_profile = NULL;  
    stream *str;
    gs_memory_t *mem_gc = icc_manager->memory;
    int code;
    int k;
    int num_comps = 0;
    gsicc_colorbuffer_t default_space;  

     
    default_space = gsUNDEFINED;
    if (icc_manager->smask_profiles !=NULL &&
        icc_manager->smask_profiles->swapped == true) {
            return 0;
    } else {
        switch(defaulttype) {
            case DEFAULT_GRAY:
                manager_default_profile = &(icc_manager->default_gray);
                default_space = gsGRAY;
                num_comps = 1;
                break;
            case DEFAULT_RGB:
                manager_default_profile = &(icc_manager->default_rgb);
                default_space = gsRGB;
                num_comps = 3;
                break;
            case DEFAULT_CMYK:
                 manager_default_profile = &(icc_manager->default_cmyk);
                 default_space = gsCMYK;
                 num_comps = 4;
                 break;
            case NAMED_TYPE:
                 manager_default_profile = &(icc_manager->device_named);
                 default_space = gsNAMED;
                 break;
            case LAB_TYPE:
                 manager_default_profile = &(icc_manager->lab_profile);
                 num_comps = 3;
                 default_space = gsCIELAB;
                 break;
            case DEVICEN_TYPE:
                manager_default_profile = NULL;
                default_space = gsNCHANNEL;
                break;
            case DEFAULT_NONE:
            default:
                return 0;
                break;
        }
    }
     
    if (defaulttype != DEVICEN_TYPE && (*manager_default_profile) != NULL) {
         
        icc_profile = *manager_default_profile;
        if ( namelen == icc_profile->name_length ) {
            if( memcmp(pname, icc_profile->name, namelen) == 0)
                return 0;
        }
        if (strncmp(icc_profile->name, OI_PROFILE,
                    strlen(icc_profile->name)) == 0) {
                return 0;
        }
        rc_decrement(icc_profile,"gsicc_set_profile");
    }
     
    if (defaulttype == DEVICEN_TYPE) {
        if (icc_manager->device_n != NULL) {
            gsicc_devicen_entry_t *current_entry = icc_manager->device_n->head;
            for (k = 0; k < icc_manager->device_n->count; k++) {
                if (current_entry->iccprofile != NULL) {
                    icc_profile = current_entry->iccprofile;
                    if (namelen == icc_profile->name_length)
                    if (memcmp(pname, icc_profile->name, namelen) == 0)
                        return 0;
                }
                current_entry = current_entry->next;
            }
        }
         
        code = gsicc_new_devicen(icc_manager);
        if (code < 0)
            return code;
        manager_default_profile = &(icc_manager->device_n->final->iccprofile);
    }
    code = gsicc_open_search(pname, namelen, mem_gc, mem_gc->gs_lib_ctx->profiledir,
                             mem_gc->gs_lib_ctx->profiledir_len, &str);
    if (code < 0)
        return code;
    if (str != NULL) {
        icc_profile = gsicc_profile_new(str, mem_gc, pname, namelen);
         
        if (icc_profile == NULL && defaulttype == NAMED_TYPE) {
             
            char *nameptr;

            icc_profile = gsicc_profile_new(NULL, mem_gc, NULL, 0);
            if (icc_profile == NULL)
                return gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
            icc_profile->data_cs = gsNAMED;
            code = gsicc_load_namedcolor_buffer(icc_profile, str, mem_gc);
            if (code < 0) return gs_throw1(-1, "problems with profile %s", pname);
            *manager_default_profile = icc_profile;
            nameptr = (char*) gs_alloc_bytes(icc_profile->memory, namelen+1,
                                             "gsicc_set_profile");
            if (nameptr == NULL)
                return gs_throw(gs_error_VMerror, "Insufficient memory for profile name");
            memcpy(nameptr, pname, namelen);
            nameptr[namelen] = '\0';
            icc_profile->name = nameptr;
            icc_profile->name_length = namelen;
            return 0;   
        }
        code = sfclose(str);
        if (icc_profile == NULL) {
            return gs_throw1(-1, "problems with profile %s",pname);
        }
         *manager_default_profile = icc_profile;
        icc_profile->default_match = defaulttype;
        if (defaulttype == LAB_TYPE)
            icc_profile->islab = true;
        if ( defaulttype == DEVICEN_TYPE ) {
             
            gsicc_get_devicen_names(icc_profile, icc_manager->memory);
             
            code = gsicc_init_profile_info(icc_profile);
            if (code < 0) return gs_throw1(-1, "problems with profile %s", pname);
        } else {
             
            icc_profile->num_comps = num_comps;
            icc_profile->num_comps_out = 3;
            gscms_set_icc_range(&icc_profile);
            icc_profile->data_cs = default_space;
        }
        return 0;
    }
    return -1;
}
