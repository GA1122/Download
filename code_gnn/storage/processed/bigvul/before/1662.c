gs_pattern1_set_color(const gs_client_color * pcc, gs_gstate * pgs)
{
    gs_pattern1_instance_t * pinst = (gs_pattern1_instance_t *)pcc->pattern;
    gs_pattern1_template_t * ptmplt = &pinst->templat;

    if (ptmplt->PaintType == 2) {
        const gs_color_space *pcs = gs_currentcolorspace_inline(pgs);

        pcs = pcs->base_space;
        return pcs->type->set_overprint(pcs, pgs);
    } else {
        gs_overprint_params_t   params;

        params.retain_any_comps = false;
        pgs->effective_overprint_mode = 0;
        return gs_gstate_update_overprint(pgs, &params);
    }
}
