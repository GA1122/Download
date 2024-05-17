static uint64_t xhci_oper_read(void *ptr, hwaddr reg, unsigned size)
{
    XHCIState *xhci = ptr;
    uint32_t ret;

    switch (reg) {
    case 0x00:  
        ret = xhci->usbcmd;
        break;
    case 0x04:  
        ret = xhci->usbsts;
        break;
    case 0x08:  
        ret = 1;  
        break;
    case 0x14:  
        ret = xhci->dnctrl;
        break;
    case 0x18:  
        ret = xhci->crcr_low & ~0xe;
        break;
    case 0x1c:  
        ret = xhci->crcr_high;
        break;
    case 0x30:  
        ret = xhci->dcbaap_low;
        break;
    case 0x34:  
        ret = xhci->dcbaap_high;
        break;
    case 0x38:  
        ret = xhci->config;
        break;
    default:
        trace_usb_xhci_unimplemented("oper read", reg);
        ret = 0;
    }

    trace_usb_xhci_oper_read(reg, ret);
    return ret;
}
