pdf14_set_marking_params(gx_device *dev, const gs_gstate *pgs)
{
    pdf14_device * pdev = (pdf14_device *)dev;

    pdev->opacity = pgs->opacity.alpha;
    pdev->shape = pgs->shape.alpha;
    pdev->alpha = pgs->opacity.alpha * pgs->shape.alpha;
    pdev->blend_mode = pgs->blend_mode;
    pdev->overprint = pgs->overprint;
    pdev->overprint_mode = pgs->overprint_mode;

    if_debug3m('v', dev->memory,
               "[v]set_marking_params, opacity = %g, shape = %g, bm = %d\n",
               pdev->opacity, pdev->shape, pgs->blend_mode);
}
