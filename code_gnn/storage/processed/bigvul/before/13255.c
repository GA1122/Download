c_pdf14trans_clist_write_update(const gs_composite_t * pcte, gx_device * dev,
                gx_device ** pcdev, gs_gstate * pgs, gs_memory_t * mem)
{
    gx_device_clist_writer * const cdev = &((gx_device_clist *)dev)->writer;
    const gs_pdf14trans_t * pdf14pct = (const gs_pdf14trans_t *) pcte;
    int code = 0;

     
    switch (pdf14pct->params.pdf14_op) {
        case PDF14_PUSH_DEVICE:
            return gs_pdf14_clist_device_push(mem, pgs, pcdev, dev, pdf14pct);

        case PDF14_POP_DEVICE:
#	    if 0  
             
            if (pgs->dev_ht)
                code = cmd_put_halftone((gx_device_clist_writer *)
                           (((pdf14_clist_device *)dev)->target), pgs->dev_ht);
#	    else
            code = 0;
#	    endif
            code = clist_writer_check_empty_cropping_stack(cdev);
            break;

        case PDF14_BEGIN_TRANS_GROUP:
            {	 
                gs_pdf14trans_t * pdf14pct_noconst;

                pdf14pct_noconst = (gs_pdf14trans_t *) pcte;
                 
                pdf14pct_noconst->params.mask_id = cdev->mask_id;
                if_debug1m('v', pgs->memory,
                           "[v]c_pdf14trans_clist_write_update group mask_id=%d \n",
                           cdev->mask_id);
            }
            break;
        case PDF14_END_TRANS_GROUP:
            code = 0;  
            break;
        case PDF14_BEGIN_TRANS_MASK:
             
            cdev->mask_id = ++cdev->mask_id_count;
             
            {	 
                gs_pdf14trans_t * pdf14pct_noconst;

                pdf14pct_noconst = (gs_pdf14trans_t *) pcte;
                pdf14pct_noconst->params.mask_id = cdev->mask_id;
                if_debug1m('v', pgs->memory,
                           "[v]c_pdf14trans_clist_write_update mask mask_id=%d \n",
                           cdev->mask_id);
            }
            break;
        case PDF14_END_TRANS_MASK:
            code = 0;  
            break;
        case PDF14_PUSH_TRANS_STATE:
            code = 0;  
            break;
        case PDF14_POP_TRANS_STATE:
            code = 0;  
            break;
        case PDF14_ABORT_DEVICE:
            code = 0;
            break;
        case PDF14_PUSH_SMASK_COLOR:
            return 0;
            break;
        case PDF14_POP_SMASK_COLOR:
            return 0;
            break;
        default:
            break;		 
    }
    *pcdev = dev;
    if (code < 0)
        return code;
     
    code = gs_gstate_setmatrix(&cdev->gs_gstate, &pdf14pct->params.ctm);
     
    cmd_clear_known(cdev, ctm_known);

    return code;
}
