pdf14_put_params(gx_device * dev, gs_param_list	* plist)
{
    pdf14_device * pdev = (pdf14_device *)dev;
    gx_device * tdev = pdev->target;
    bool was_open = tdev->is_open;
    int code = 0;

    if (tdev != 0 && (code = dev_proc(tdev, put_params)(tdev, plist)) >= 0) {
        gx_device_decache_colors(dev);
        if (!tdev->is_open) {
            code = gs_closedevice(dev);
            if (code == 0)
                code = was_open ? 1 : 0;    
        }
        gs_pdf14_device_copy_params(dev, tdev);
    }
    return code;
}
