pdf14_close(gx_device *dev)
{
    pdf14_device *pdev = (pdf14_device *)dev;

    pdf14_cleanup_parent_color_profiles (pdev);

    if (pdev->ctx) {
        pdf14_ctx_free(pdev->ctx);
        pdev->ctx = NULL;
    }
    return 0;
}
