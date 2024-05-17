static uint64_t xhci_port_read(void *ptr, hwaddr reg, unsigned size)
{
    XHCIPort *port = ptr;
    uint32_t ret;

    switch (reg) {
    case 0x00:  
        ret = port->portsc;
        break;
    case 0x04:  
    case 0x08:  
        ret = 0;
        break;
    case 0x0c:  
    default:
        trace_usb_xhci_unimplemented("port read", reg);
        ret = 0;
    }

    trace_usb_xhci_port_read(port->portnr, reg, ret);
    return ret;
}
