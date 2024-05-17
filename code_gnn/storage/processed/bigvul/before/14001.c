gsicc_set_srcgtag_struct(gsicc_manager_t *icc_manager, const char* pname,
                        int namelen)
{
    gs_memory_t *mem;
    stream *str;
    int code;
    int info_size;
    char *buffer_ptr, *curr_ptr, *last;
    int num_bytes;
    int k;
    static const char *const srcgtag_keys[] = {GSICC_SRCGTAG_KEYS};
    cmm_profile_t *icc_profile;
    cmm_srcgtag_profile_t *srcgtag;
    bool start = true;
    gsicc_cmm_t cmm = gsCMM_DEFAULT;

     
    if (icc_manager == NULL || icc_manager->srcgtag_profile != NULL) {
        return 0;
    } else {
        mem = icc_manager->memory->non_gc_memory;
        code = gsicc_open_search(pname, namelen, mem, mem->gs_lib_ctx->profiledir,
                                 mem->gs_lib_ctx->profiledir_len, &str);
        if (code < 0)
            return code;
    }
    if (str != NULL) {
         
        code = sfseek(str,0,SEEK_END);
        if (code < 0)
            return code;
        info_size = sftell(str);
        code = srewind(str);
        if (code < 0)
            return code;
        if (info_size > (GSICC_NUM_SRCGTAG_KEYS + 1) * FILENAME_MAX) {
            return gs_throw1(-1, "setting of %s src obj color info failed",
                               pname);
        }
         
        buffer_ptr = (char*) gs_alloc_bytes(mem, info_size+1,
                                            "gsicc_set_srcgtag_struct");
        if (buffer_ptr == NULL) {
            return gs_throw1(gs_error_VMerror, "setting of %s src obj color info failed",
                               pname);
        }
        num_bytes = sfread(buffer_ptr,sizeof(unsigned char), info_size, str);
        code = sfclose(str);
        if (code < 0)
            return code;
        buffer_ptr[info_size] = 0;
        if (num_bytes != info_size) {
            gs_free_object(mem, buffer_ptr, "gsicc_set_srcgtag_struct");
            return gs_throw1(-1, "setting of %s src obj color info failed",
                               pname);
        }
         
        srcgtag = gsicc_new_srcgtag_profile(mem);
         
        curr_ptr = buffer_ptr;
         
        for (k = 0; k < NUM_SOURCE_PROFILES; k++) {
            srcgtag->rgb_rend_cond[k].cmm = gsCMM_DEFAULT;
            srcgtag->cmyk_rend_cond[k].cmm = gsCMM_DEFAULT;
            srcgtag->gray_rend_cond[k].cmm = gsCMM_DEFAULT;
        }
        while (start || strlen(curr_ptr) > 0) {
            if (start) {
                curr_ptr = gs_strtok(buffer_ptr, "\t,\32\n\r", &last);
                start = false;
            } else {
                curr_ptr = gs_strtok(NULL, "\t,\32\n\r", &last);
            }
            if (curr_ptr == NULL) break;
             
            for (k = 0; k < GSICC_NUM_SRCGTAG_KEYS; k++) {
                if (strncmp(curr_ptr, srcgtag_keys[k], strlen(srcgtag_keys[k])) == 0 ) {
                     
                    curr_ptr = gs_strtok(NULL, "\t,\32\n\r", &last);
                    if (strncmp(curr_ptr, GSICC_SRCTAG_NOCM, strlen(GSICC_SRCTAG_NOCM)) == 0 &&
                        strlen(curr_ptr) == strlen(GSICC_SRCTAG_NOCM)) {
                        cmm = gsCMM_NONE;
                        icc_profile = NULL;
                        break;
                    } else if ((strncmp(curr_ptr, GSICC_SRCTAG_REPLACE, strlen(GSICC_SRCTAG_REPLACE)) == 0 &&
                        strlen(curr_ptr) == strlen(GSICC_SRCTAG_REPLACE))) {
                        cmm = gsCMM_REPLACE;
                        icc_profile = NULL;
                        break;
                    } else {
                         
                        code = gsicc_open_search(curr_ptr, strlen(curr_ptr), mem,
                                                 mem->gs_lib_ctx->profiledir,
                                                 mem->gs_lib_ctx->profiledir_len, &str);
                        if (code < 0)
                            return code;
                        if (str != NULL) {
                            icc_profile =
                                gsicc_profile_new(str, mem, curr_ptr, strlen(curr_ptr));
                            code = sfclose(str);
                            if (code < 0)
                                return code;
                        }
                        if (str != NULL && icc_profile != NULL) {
                            code = gsicc_init_profile_info(icc_profile);
                            if (code < 0)
                                return code;
                            cmm = gsCMM_DEFAULT;
                             
                            gsicc_check_device_link(icc_profile);
                            break;
                        } else {
                             
                            gs_free_object(mem, buffer_ptr, "gsicc_set_srcgtag_struct");
                            rc_decrement(srcgtag, "gsicc_set_srcgtag_struct");
                            return gs_throw1(-1,
                                    "setting of %s src obj color info failed", pname);
                        }
                    }
                }
            }
             
            switch (k) {
                case COLOR_TUNE:
                     
                    srcgtag->color_warp_profile = icc_profile;
                    break;
                case GRAPHIC_CMYK:
                    srcgtag->cmyk_profiles[gsSRC_GRAPPRO] = icc_profile;
                    srcgtag->cmyk_rend_cond[gsSRC_GRAPPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->cmyk_rend_cond[gsSRC_GRAPPRO]), &last, true);
                        if (code < 0)
                            return code;
                    }
                    break;
                case IMAGE_CMYK:
                    srcgtag->cmyk_profiles[gsSRC_IMAGPRO] = icc_profile;
                    srcgtag->cmyk_rend_cond[gsSRC_IMAGPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->cmyk_rend_cond[gsSRC_IMAGPRO]), &last, true);
                        if (code < 0)
                            return code;
                    }
                    break;
                case TEXT_CMYK:
                    srcgtag->cmyk_profiles[gsSRC_TEXTPRO] = icc_profile;
                    srcgtag->cmyk_rend_cond[gsSRC_TEXTPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->cmyk_rend_cond[gsSRC_TEXTPRO]), &last, true);
                        if (code < 0)
                            return code;
                    }
                    break;
                case GRAPHIC_RGB:
                    srcgtag->rgb_profiles[gsSRC_GRAPPRO] = icc_profile;
                    srcgtag->rgb_rend_cond[gsSRC_GRAPPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->rgb_rend_cond[gsSRC_GRAPPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                   break;
                case IMAGE_RGB:
                    srcgtag->rgb_profiles[gsSRC_IMAGPRO] = icc_profile;
                    srcgtag->rgb_rend_cond[gsSRC_IMAGPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->rgb_rend_cond[gsSRC_IMAGPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                    break;
                case TEXT_RGB:
                    srcgtag->rgb_profiles[gsSRC_TEXTPRO] = icc_profile;
                    srcgtag->rgb_rend_cond[gsSRC_TEXTPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->rgb_rend_cond[gsSRC_TEXTPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                    break;
                case GRAPHIC_GRAY:
                    srcgtag->gray_profiles[gsSRC_GRAPPRO] = icc_profile;
                    srcgtag->gray_rend_cond[gsSRC_GRAPPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->gray_rend_cond[gsSRC_GRAPPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                    break;
                case IMAGE_GRAY:
                    srcgtag->gray_profiles[gsSRC_IMAGPRO] = icc_profile;
                    srcgtag->gray_rend_cond[gsSRC_IMAGPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->gray_rend_cond[gsSRC_IMAGPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                    break;
                case TEXT_GRAY:
                    srcgtag->gray_profiles[gsSRC_TEXTPRO] = icc_profile;
                    srcgtag->gray_rend_cond[gsSRC_TEXTPRO].cmm = cmm;
                    if (cmm == gsCMM_DEFAULT) {
                        code = gsicc_fill_srcgtag_item(&(srcgtag->gray_rend_cond[gsSRC_TEXTPRO]), &last, false);
                        if (code < 0)
                            return code;
                    }
                    break;
                case GSICC_NUM_SRCGTAG_KEYS:
                     
                    gs_free_object(mem, buffer_ptr, "gsicc_set_srcgtag_struct");
                    rc_decrement(srcgtag, "gsicc_set_srcgtag_struct");
                    return gs_throw1(-1, "failed to find key in %s", pname);
                    break;
                default:
                     
                    gs_free_object(mem, buffer_ptr, "gsicc_set_srcgtag_struct");
                    rc_decrement(srcgtag, "gsicc_set_srcgtag_struct");
                    return gs_throw1(-1, "Error in srcgtag data %s", pname);
                    break;
            }
        }
    } else {
        return gs_throw1(-1, "setting of %s src obj color info failed", pname);
    }
    gs_free_object(mem, buffer_ptr, "gsicc_set_srcgtag_struct");
    srcgtag->name_length = strlen(pname);
    srcgtag->name = (char*) gs_alloc_bytes(mem, srcgtag->name_length,
                                  "gsicc_set_srcgtag_struct");
    if (srcgtag->name == NULL)
        return gs_throw(gs_error_VMerror, "Insufficient memory for tag name");
    strncpy(srcgtag->name, pname, srcgtag->name_length);
    icc_manager->srcgtag_profile = srcgtag;
    return 0;
}
