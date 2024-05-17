static TRBCCode xhci_alloc_device_streams(XHCIState *xhci, unsigned int slotid,
                                          uint32_t epmask)
{
    XHCIEPContext *epctxs[30];
    USBEndpoint *eps[30];
    int i, r, nr_eps, req_nr_streams, dev_max_streams;

    nr_eps = xhci_epmask_to_eps_with_streams(xhci, slotid, epmask, epctxs,
                                             eps);
    if (nr_eps == 0) {
        return CC_SUCCESS;
    }

    req_nr_streams = epctxs[0]->nr_pstreams;
    dev_max_streams = eps[0]->max_streams;

    for (i = 1; i < nr_eps; i++) {
         
        if (epctxs[i]->nr_pstreams != req_nr_streams) {
            FIXME("guest streams config not identical for all eps");
            return CC_RESOURCE_ERROR;
        }
        if (eps[i]->max_streams != dev_max_streams) {
            FIXME("device streams config not identical for all eps");
            return CC_RESOURCE_ERROR;
        }
    }

     
    if (req_nr_streams > dev_max_streams) {
        req_nr_streams = dev_max_streams;
    }

    r = usb_device_alloc_streams(eps[0]->dev, eps, nr_eps, req_nr_streams);
    if (r != 0) {
        DPRINTF("xhci: alloc streams failed\n");
        return CC_RESOURCE_ERROR;
    }

    return CC_SUCCESS;
}
