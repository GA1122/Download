c_pdf14trans_write(const gs_composite_t	* pct, byte * data, uint * psize,
                   gx_device_clist_writer *cdev)
{
    const gs_pdf14trans_params_t * pparams = &((const gs_pdf14trans_t *)pct)->params;
    int need, avail = *psize;
    byte buf[MAX_CLIST_TRANSPARENCY_BUFFER_SIZE];  
    byte * pbuf = buf;
    int opcode = pparams->pdf14_op;
    int mask_size = 0;
    uint mask_id = 0;
    int code;
    bool found_icc;
    int64_t hashcode = 0;
    cmm_profile_t *icc_profile;
    gsicc_rendering_param_t render_cond;
    cmm_dev_profile_t *dev_profile;
     
    int pdf14_needed = cdev->pdf14_needed;
    int trans_group_level = cdev->pdf14_trans_group_level;
    int smask_level = cdev->pdf14_smask_level;

    code = dev_proc((gx_device *) cdev, get_profile)((gx_device *) cdev,
                                                     &dev_profile);
    if (code < 0)
        return code;
    gsicc_extract_profile(GS_UNKNOWN_TAG, dev_profile, &icc_profile,
                          &render_cond);
    *pbuf++ = opcode;			 
    switch (opcode) {
        default:			 
            break;
        case PDF14_PUSH_DEVICE:
            trans_group_level = 0;
            cdev->pdf14_smask_level = 0;
            cdev->page_pdf14_needed = false;
            put_value(pbuf, pparams->num_spot_colors);
            put_value(pbuf, pparams->is_pattern);
             
            if (icc_profile->data_cs == gsCIELAB || icc_profile->islab) {
                 
                cdev->trans_dev_icc_hash = pparams->iccprofile->hashcode;
                found_icc =
                    clist_icc_searchtable(cdev, pparams->iccprofile->hashcode);
                if (!found_icc) {
                     
                    clist_icc_addentry(cdev, pparams->iccprofile->hashcode,
                                       pparams->iccprofile);
                }
            }
            break;
        case PDF14_POP_DEVICE:
            pdf14_needed = false;		 
            trans_group_level = 0;
            smask_level = 0;
            put_value(pbuf, pparams->is_pattern);
            break;
        case PDF14_END_TRANS_GROUP:
            trans_group_level--;	 
            if (smask_level == 0 && trans_group_level == 0)
                pdf14_needed = cdev->page_pdf14_needed;
            break;			 
        case PDF14_BEGIN_TRANS_GROUP:
            pdf14_needed = true;		 
            trans_group_level++;
            code = c_pdf14trans_write_ctm(&pbuf, pparams);
            if (code < 0)
                return code;
            *pbuf++ = (pparams->Isolated & 1) + ((pparams->Knockout & 1) << 1);
            *pbuf++ = pparams->blend_mode;
            *pbuf++ = pparams->group_color;
            put_value(pbuf, pparams->group_color_numcomps);
            put_value(pbuf, pparams->opacity.alpha);
            put_value(pbuf, pparams->shape.alpha);
            put_value(pbuf, pparams->bbox);
            mask_id = pparams->mask_id;
            put_value(pbuf, mask_id);
             
            if (pparams->group_color == ICC) {
                 
                hashcode = pparams->iccprofile->hashcode;
                found_icc = clist_icc_searchtable(cdev, hashcode);
                if (!found_icc) {
                     
                    clist_icc_addentry(cdev, hashcode, pparams->iccprofile);
                    put_value(pbuf, hashcode);
                } else {
                     
                    put_value(pbuf, hashcode);
                }
            } else {
                put_value(pbuf, hashcode);
            }
            break;
        case PDF14_BEGIN_TRANS_MASK:
            if (pparams->subtype != TRANSPARENCY_MASK_None) {
                pdf14_needed = true;		 
                smask_level++;
            }
            code = c_pdf14trans_write_ctm(&pbuf, pparams);
            if (code < 0)
                return code;
            put_value(pbuf, pparams->subtype);
            *pbuf++ = pparams->group_color;
            put_value(pbuf, pparams->group_color_numcomps);
            *pbuf++ = pparams->replacing;
            *pbuf++ = pparams->function_is_identity;
            *pbuf++ = pparams->Background_components;
            *pbuf++ = pparams->Matte_components;
            put_value(pbuf, pparams->bbox);
            mask_id = pparams->mask_id;
            put_value(pbuf, mask_id);
            if (pparams->Background_components) {
                const int l = sizeof(pparams->Background[0]) * pparams->Background_components;

                memcpy(pbuf, pparams->Background, l);
                pbuf += l;
                memcpy(pbuf, &pparams->GrayBackground, sizeof(pparams->GrayBackground));
                pbuf += sizeof(pparams->GrayBackground);
            }
            if (pparams->Matte_components) {
                const int m = sizeof(pparams->Matte[0]) * pparams->Matte_components;

                memcpy(pbuf, pparams->Matte, m);
                pbuf += m;
            }
            if (!pparams->function_is_identity)
                mask_size = sizeof(pparams->transfer_fn);
             
            if (pparams->group_color == ICC) {
                 
                hashcode = pparams->iccprofile->hashcode;
                found_icc = clist_icc_searchtable(cdev, hashcode);
                if (!found_icc) {
                     
                    clist_icc_addentry(cdev, hashcode, pparams->iccprofile);
                    put_value(pbuf, hashcode);
                } else {
                     
                    put_value(pbuf, hashcode);
                }
            } else {
                put_value(pbuf, hashcode);
            }
            break;
        case PDF14_END_TRANS_MASK:
            smask_level--;
            if (smask_level == 0 && trans_group_level == 0)
                pdf14_needed = cdev->page_pdf14_needed;
            break;
        case PDF14_SET_BLEND_PARAMS:
            if (pparams->blend_mode != BLEND_MODE_Normal || pparams->opacity.alpha != 1.0 ||
                pparams->shape.alpha != 1.0)
                pdf14_needed = true;		 
            else if (smask_level == 0 && trans_group_level == 0)
                pdf14_needed = false;		 
            if (smask_level == 0 && trans_group_level == 0)
                cdev->page_pdf14_needed = pdf14_needed;          
            *pbuf++ = pparams->changed;
            if (pparams->changed & PDF14_SET_BLEND_MODE)
                *pbuf++ = pparams->blend_mode;
            if (pparams->changed & PDF14_SET_TEXT_KNOCKOUT)
                *pbuf++ = pparams->text_knockout;
            if (pparams->changed & PDF14_SET_OPACITY_ALPHA)
                put_value(pbuf, pparams->opacity.alpha);
            if (pparams->changed & PDF14_SET_SHAPE_ALPHA)
                put_value(pbuf, pparams->shape.alpha);
            if (pparams->changed & PDF14_SET_OVERPRINT)
                put_value(pbuf, pparams->overprint);
            if (pparams->changed & PDF14_SET_OVERPRINT_MODE)
                put_value(pbuf, pparams->overprint_mode);
            break;
        case PDF14_PUSH_TRANS_STATE:
            break;
        case PDF14_POP_TRANS_STATE:
            break;
        case PDF14_PUSH_SMASK_COLOR:
            return 0;    
            break;
        case PDF14_POP_SMASK_COLOR:
            return 0;    
            break;
    }

     
    need = (pbuf - buf) + mask_size;
    *psize = need;
    if (need > avail) {
        if (avail)
            return_error(gs_error_rangecheck);
        else
            return gs_error_rangecheck;
    }

     
    if ((need + 3 > MAX_CLIST_COMPOSITOR_SIZE) ||
        (need + 3 - mask_size > MAX_CLIST_TRANSPARENCY_BUFFER_SIZE) )
        return_error(gs_error_rangecheck);

     
    memcpy(data, buf, need - mask_size);
    if (mask_size)	 
        memcpy(data + need - mask_size, pparams->transfer_fn, mask_size);
    if_debug3m('v', cdev->memory,
               "[v] c_pdf14trans_write: opcode = %s mask_id=%d need = %d\n",
               pdf14_opcode_names[opcode], mask_id, need);
    cdev->pdf14_needed = pdf14_needed;           
    cdev->pdf14_trans_group_level = trans_group_level;
    cdev->pdf14_smask_level = smask_level;
    return 0;
}