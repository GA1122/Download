pdf14_set_params(gs_gstate * pgs,	gx_device * dev,
                                const gs_pdf14trans_params_t * pparams)
{
    if_debug0m('v', dev->memory, "[v]pdf14_set_params\n");
    if (pparams->changed & PDF14_SET_BLEND_MODE)
        pgs->blend_mode = pparams->blend_mode;
    if (pparams->changed & PDF14_SET_TEXT_KNOCKOUT)
        pgs->text_knockout = pparams->text_knockout;
    if (pparams->changed & PDF14_SET_SHAPE_ALPHA)
        pgs->shape.alpha = pparams->shape.alpha;
    if (pparams->changed & PDF14_SET_OPACITY_ALPHA)
        pgs->opacity.alpha = pparams->opacity.alpha;
    if (pparams->changed & PDF14_SET_OVERPRINT)
        pgs->overprint = pparams->overprint;
    if (pparams->changed & PDF14_SET_OVERPRINT_MODE)
        pgs->overprint_mode = pparams->overprint_mode;
    pdf14_set_marking_params(dev, pgs);
}
