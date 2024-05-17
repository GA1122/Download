c_pdf14trans_get_cropping(const gs_composite_t *pcte, int *ry, int *rheight,
                          int cropping_min, int cropping_max)
{
    gs_pdf14trans_t * pdf14pct = (gs_pdf14trans_t *) pcte;
    switch (pdf14pct->params.pdf14_op) {
        case PDF14_PUSH_DEVICE: return ALLBANDS;  
        case PDF14_POP_DEVICE:  return ALLBANDS;  
        case PDF14_ABORT_DEVICE: return ALLBANDS;  
        case PDF14_BEGIN_TRANS_GROUP:
            {	gs_int_rect rect;

                pdf14_compute_group_device_int_rect(&pdf14pct->params.ctm,
                                                &pdf14pct->params.bbox, &rect);
                 
                *ry = max(rect.p.y, cropping_min);
                *rheight = min(rect.q.y, cropping_max) - *ry;
                return PUSHCROP;  
            }
        case PDF14_BEGIN_TRANS_MASK:
            {	gs_int_rect rect;

                pdf14_compute_group_device_int_rect(&pdf14pct->params.ctm,
                                                    &pdf14pct->params.bbox, &rect);
                 
                if ( pdf14pct->params.GrayBackground == 1.0 || pdf14pct->params.mask_is_image) {
                     
                    *ry = max(rect.p.y, cropping_min);
                    *rheight = min(rect.q.y, cropping_max) - *ry;
                    return PUSHCROP;  
                }  else {
                     
                    *ry = cropping_min;
                    *rheight = cropping_max - cropping_min;
                    if (pdf14pct->params.subtype == TRANSPARENCY_MASK_None)
                        return SAMEAS_PUSHCROP_BUTNOPUSH;
                    else
                        return PUSHCROP;  
                }
            }
        case PDF14_END_TRANS_GROUP: return POPCROP;  
        case PDF14_END_TRANS_MASK: return POPCROP;    
        case PDF14_PUSH_TRANS_STATE: return CURRBANDS;
        case PDF14_POP_TRANS_STATE: return CURRBANDS;
        case PDF14_SET_BLEND_PARAMS: return ALLBANDS;
        case PDF14_PUSH_SMASK_COLOR: return POPCROP;  
        case PDF14_POP_SMASK_COLOR: return POPCROP;    
    }
    return ALLBANDS;
}
