pdf14_create_compositor(gx_device * dev, gx_device * * pcdev,
        const gs_composite_t * pct, gs_gstate * pgs,
        gs_memory_t * mem, gx_device *cdev)
{
    pdf14_device *p14dev = (pdf14_device *)dev;
    if (gs_is_pdf14trans_compositor(pct)) {
        const gs_pdf14trans_t * pdf14pct = (const gs_pdf14trans_t *) pct;
        *pcdev = dev;
         
        p14dev->pclist_device = cdev;
        return gx_update_pdf14_compositor(dev, pgs, pdf14pct, mem);
    } else if (gs_is_overprint_compositor(pct)) {
                 
                const gs_overprint_t * op_pct = (const gs_overprint_t *) pct;
                if (op_pct->params.retain_any_comps && !op_pct->params.retain_spot_comps) {
                    p14dev->drawn_comps = op_pct->params.drawn_comps;
                } else {
                     
                    p14dev->drawn_comps = ( (gx_color_index) 1 << (p14dev->color_info.num_components)) - (gx_color_index) 1;
                }
                *pcdev = dev;
                return 0;
    } else
        return gx_no_create_compositor(dev, pcdev, pct, pgs, mem, cdev);
}
