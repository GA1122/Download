static uint64_t xhci_doorbell_read(void *ptr, hwaddr reg,
                                   unsigned size)
{
     
    trace_usb_xhci_doorbell_read(reg, 0);
    return 0;
}
