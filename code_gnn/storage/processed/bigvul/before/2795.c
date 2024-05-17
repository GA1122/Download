gs_setdevice_no_erase(gs_gstate * pgs, gx_device * dev)
{
    int open_code = 0, code;
    gs_lib_ctx_t *libctx = gs_lib_ctx_get_interp_instance(pgs->memory);

     
    if (libctx->io_device_table != NULL) {
        cmm_dev_profile_t *dev_profile;
        if (pgs->icc_manager->lab_profile == NULL) {   
            gsicc_init_iccmanager(pgs);
        }
         
         
        if (dev->procs.get_profile != NULL) {
            code = dev_proc(dev, get_profile)(dev, &dev_profile);
            if (code < 0) {
                return(code);
            }
            if (dev_profile == NULL ||
                dev_profile->device_profile[gsDEFAULTPROFILE] == NULL) {
                if ((code = gsicc_init_device_profile_struct(dev, NULL,
                                                        gsDEFAULTPROFILE)) < 0)
                    return(code);
                 
                if ((code = gsicc_set_device_profile_intent(dev, gsRINOTSPECIFIED,
                                                       gsDEFAULTPROFILE)) < 0)
                    return(code);
            }
        }
    }

     
    if (!dev->is_open) {
        gx_device_fill_in_procs(dev);

         
        if (libctx->io_device_table != NULL) {
            cmm_dev_profile_t *dev_profile;
             
            if (dev->procs.get_profile != NULL) {
                code = dev_proc(dev, get_profile)(dev, &dev_profile);
                if (code < 0) {
                    return(code);
                }
                if (dev_profile == NULL ||
                    dev_profile->device_profile[gsDEFAULTPROFILE] == NULL) {
                    if ((code = gsicc_init_device_profile_struct(dev, NULL,
                                                            gsDEFAULTPROFILE)) < 0)
                        return(code);
                }
            }
        }

        if (gs_device_is_memory(dev)) {
             
            gx_device *odev = gs_currentdevice_inline(pgs);

            while (odev != 0 && gs_device_is_memory(odev))
                odev = ((gx_device_memory *)odev)->target;
            gx_device_set_target(((gx_device_forward *)dev), odev);
        }
        code = open_code = gs_opendevice(dev);
        if (code < 0)
            return code;
    }
    gs_setdevice_no_init(pgs, dev);
    pgs->ctm_default_set = false;
    if ((code = gs_initmatrix(pgs)) < 0 ||
        (code = gs_initclip(pgs)) < 0
        )
        return code;
     
     
    pgs->in_cachedevice = 0;
    pgs->in_charpath = (gs_char_path_mode) 0;
    return open_code;
}
