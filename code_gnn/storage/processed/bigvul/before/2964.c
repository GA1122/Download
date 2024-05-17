pdf14_pattern_trans_render(gx_image_enum * penum, const byte * buffer, int data_x,
                    uint w, int h, gx_device * dev)
{
    int code;
    pdf14_device * p14dev;
    const gs_gstate * pgs = penum->pgs;
    gx_device_color * pdcolor = (penum->icolor1);
    gx_color_tile *ptile = pdcolor->colors.pattern.p_tile;

     
    code = (ptile->ttrans->image_render)(penum, buffer, data_x, w, h, dev);
    if (code < 0)
        return code;
     
    if (h == 0 && ptile->trans_group_popped == false) {
        p14dev = find_pdf14_device(dev);

        if (p14dev->pclist_device == NULL) {
             
            code = gs_end_transparency_group((gs_gstate *) pgs);
        } else {
             
            cmm_dev_profile_t *dev_profile;
            code = dev_proc(dev, get_profile)(dev,  &dev_profile);
            if (code < 0)
                return code;

            if_debug2m('v', p14dev->ctx->memory,
                      "[v*] Popping trans group pattern fill, uid = %ld id = %ld \n",
                       ptile->uid.id, ptile->id);
            code = pdf14_pop_transparency_group(NULL, p14dev->ctx, p14dev->blend_procs,
                    p14dev->color_info.num_components, dev_profile->device_profile[0],
                    (gx_device *) p14dev);
        }
        pdcolor->colors.pattern.p_tile->trans_group_popped = true;
        gs_free_object(pgs->memory, ptile->ttrans->fill_trans_buffer,
                       "pdf14_pattern_trans_render");
        ptile->ttrans->fill_trans_buffer = NULL;   
    }
    return code;
}