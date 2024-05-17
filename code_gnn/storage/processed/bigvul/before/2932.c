pdf14_begin_typed_image(gx_device * dev, const gs_gstate * pgs,
                           const gs_matrix *pmat, const gs_image_common_t *pic,
                           const gs_int_rect * prect,
                           const gx_drawing_color * pdcolor,
                           const gx_clip_path * pcpath, gs_memory_t * mem,
                           gx_image_enum_common_t ** pinfo)
{
    const gs_image_t *pim = (const gs_image_t *)pic;
    int code;

     
    if (pim->ImageMask) {
        if (pdcolor != NULL && gx_dc_is_pattern1_color(pdcolor)) {
            if( gx_pattern1_get_transptr(pdcolor) != NULL){
                 
                if (dev_proc(dev, begin_image) != gx_default_begin_image) {
                    code = pdf14_patt_trans_image_fill(dev, pgs, pmat, pic,
                                                prect, pdcolor, pcpath, mem,
                                                pinfo);
                    return code;
                }
            }
        }
    }
    pdf14_set_marking_params(dev, pgs);
    return gx_default_begin_typed_image(dev, pgs, pmat, pic, prect, pdcolor,
                                        pcpath, mem, pinfo);
}
