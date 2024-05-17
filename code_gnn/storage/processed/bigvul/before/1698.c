static int pixmap_remap_image_pattern(const gs_client_color *pcc, gs_gstate *pgs)
{
    const gs_client_pattern *ppat = gs_getpattern(pcc);
    int code = 0;

     
    if (pgs->have_pattern_streams)
        code = dev_proc(pcc->pattern->saved->device, dev_spec_op)(pcc->pattern->saved->device,
                                gxdso_pattern_can_accum, (void *)ppat, ppat->uid.id);

    if (code == 1) {
         
        return_error(gs_error_Remap_Color);
    } else {
        return image_PaintProc(pcc, pgs);
    }
}
