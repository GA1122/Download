gs_setdevice_no_init(gs_gstate * pgs, gx_device * dev)
{
     
    if (pgs->device != NULL && pgs->device->rc.ref_count == 1 &&
        pgs->device != dev) {
        int code = gs_closedevice(pgs->device);

        if (code < 0)
            return code;
    }
    rc_assign(pgs->device, dev, "gs_setdevice_no_init");
    gs_gstate_update_device(pgs, dev);
    return pgs->overprint ? gs_do_set_overprint(pgs) : 0;
}
