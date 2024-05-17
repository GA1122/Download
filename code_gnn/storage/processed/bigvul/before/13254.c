c_pdf14trans_clist_read_update(gs_composite_t *	pcte, gx_device	* cdev,
                gx_device * tdev, gs_gstate * pgs, gs_memory_t * mem)
{
    pdf14_device * p14dev = (pdf14_device *)tdev;
    gs_pdf14trans_t * pdf14pct = (gs_pdf14trans_t *) pcte;
    gs_devn_params * pclist_devn_params;
    gx_device_clist_reader *pcrdev = (gx_device_clist_reader *)cdev;
    cmm_profile_t *cl_icc_profile, *p14_icc_profile;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;

    dev_proc(cdev, get_profile)(cdev,  &dev_profile);
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &cl_icc_profile,
                          &render_cond);
    dev_proc(p14dev, get_profile)((gx_device *)p14dev,  &dev_profile);
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &p14_icc_profile,
                          &render_cond);
     
    switch (pdf14pct->params.pdf14_op) {
        case PDF14_PUSH_DEVICE:
             
#if !CMM_THREAD_SAFE
            gx_monitor_enter(p14_icc_profile->lock);
            rc_assign(p14dev->icc_struct->device_profile[0], cl_icc_profile,
                      "c_pdf14trans_clist_read_update");
            gx_monitor_leave(p14_icc_profile->lock);
#endif
             
            pclist_devn_params = dev_proc(cdev, ret_devn_params)(cdev);
            if (pclist_devn_params != NULL && pclist_devn_params->page_spot_colors != 0) {
                int num_comp = p14dev->color_info.num_components;
                 
                p14dev->devn_params.page_spot_colors =
                    pclist_devn_params->page_spot_colors;
                if (num_comp < p14dev->devn_params.page_spot_colors + 4 ) {
                    p14dev->color_info.num_components = num_comp;
                } else {
                    p14dev->color_info.num_components =
                        p14dev->devn_params.num_std_colorant_names +
                        p14dev->devn_params.page_spot_colors;
                }
                 
                devn_free_params((gx_device*) p14dev);
                p14dev->devn_params.separations =
                    pclist_devn_params->pdf14_separations;
                p14dev->free_devicen = false;   
                if (num_comp != p14dev->color_info.num_components) {
                     
                    if (p14dev->ctx != NULL) {
                        pdf14_ctx_free(p14dev->ctx);
                    }
                    dev_proc(tdev, open_device) (tdev);
                }
            }
             
            if (cl_icc_profile->data_cs == gsCIELAB || cl_icc_profile->islab) {
                cl_icc_profile =
                    gsicc_read_serial_icc(cdev, pcrdev->trans_dev_icc_hash);
                 
                cl_icc_profile->dev = (gx_device *) cdev;
                gx_monitor_enter(p14_icc_profile->lock);
                rc_assign(p14dev->icc_struct->device_profile[0], cl_icc_profile,
                          "c_pdf14trans_clist_read_update");
                 
                rc_decrement(p14dev->icc_struct->device_profile[0],
                             "c_pdf14trans_clist_read_update");
                gx_monitor_leave(p14_icc_profile->lock);
            }
            break;

        case PDF14_POP_DEVICE:
#	    if 0  
            cdev->color_info = p14dev->saved_target_color_info;
#	    endif
            break;

        default:
            break;		 
    }

    return 0;
}
