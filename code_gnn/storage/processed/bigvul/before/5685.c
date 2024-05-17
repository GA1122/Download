static uint64_t xhci_cap_read(void *ptr, hwaddr reg, unsigned size)
{
    XHCIState *xhci = ptr;
    uint32_t ret;

    switch (reg) {
    case 0x00:  
        ret = 0x01000000 | LEN_CAP;
        break;
    case 0x04:  
        ret = ((xhci->numports_2+xhci->numports_3)<<24)
            | (xhci->numintrs<<8) | xhci->numslots;
        break;
    case 0x08:  
        ret = 0x0000000f;
        break;
    case 0x0c:  
        ret = 0x00000000;
        break;
    case 0x10:  
        if (sizeof(dma_addr_t) == 4) {
            ret = 0x00080000 | (xhci->max_pstreams_mask << 12);
        } else {
            ret = 0x00080001 | (xhci->max_pstreams_mask << 12);
        }
        break;
    case 0x14:  
        ret = OFF_DOORBELL;
        break;
    case 0x18:  
        ret = OFF_RUNTIME;
        break;

     
    case 0x20:  
        ret = 0x02000402;  
        break;
    case 0x24:  
        ret = 0x20425355;  
        break;
    case 0x28:  
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            ret = (xhci->numports_2<<8) | (xhci->numports_3+1);
        } else {
            ret = (xhci->numports_2<<8) | 1;
        }
        break;
    case 0x2c:  
        ret = 0x00000000;  
        break;
    case 0x30:  
        ret = 0x03000002;  
        break;
    case 0x34:  
        ret = 0x20425355;  
        break;
    case 0x38:  
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            ret = (xhci->numports_3<<8) | 1;
        } else {
            ret = (xhci->numports_3<<8) | (xhci->numports_2+1);
        }
        break;
    case 0x3c:  
        ret = 0x00000000;  
        break;
    default:
        trace_usb_xhci_unimplemented("cap read", reg);
        ret = 0;
    }

    trace_usb_xhci_cap_read(reg, ret);
    return ret;
}
