pdf14_clist_forward_create_compositor(gx_device	* dev, gx_device * * pcdev,
        const gs_composite_t * pct, gs_gstate * pgs,
        gs_memory_t * mem, gx_device *cdev)
{
    pdf14_device *pdev = (pdf14_device *)dev;
    gx_device * tdev = pdev->target;
    gx_device * ndev;
    int code;

    *pcdev = dev;
    if (gs_is_pdf14trans_compositor(pct)) {
        const gs_pdf14trans_t * pdf14pct = (const gs_pdf14trans_t *) pct;

        if (pdf14pct->params.pdf14_op == PDF14_PUSH_DEVICE)
            return pdf14_clist_create_compositor(dev, &ndev, pct, pgs, mem, cdev);
        return 0;
    }
    code = dev_proc(tdev, create_compositor)(tdev, &ndev, pct, pgs, mem, cdev);
    if (code < 0)
        return code;
    gx_device_set_target((gx_device_forward *)pdev, ndev);
    return 0;
}
