static uint64_t ohci_mem_read(void *opaque,
                              hwaddr addr,
                              unsigned size)
{
    OHCIState *ohci = opaque;
    uint32_t retval;

     
    if (addr & 3) {
        trace_usb_ohci_mem_read_unaligned(addr);
        return 0xffffffff;
    } else if (addr >= 0x54 && addr < 0x54 + ohci->num_ports * 4) {
         
        retval = ohci->rhport[(addr - 0x54) >> 2].ctrl | OHCI_PORT_PPS;
    } else {
        switch (addr >> 2) {
        case 0:  
            retval = 0x10;
            break;

        case 1:  
            retval = ohci->ctl;
            break;

        case 2:  
            retval = ohci->status;
            break;

        case 3:  
            retval = ohci->intr_status;
            break;

        case 4:  
        case 5:  
            retval = ohci->intr;
            break;

        case 6:  
            retval = ohci->hcca;
            break;

        case 7:  
            retval = ohci->per_cur;
            break;

        case 8:  
            retval = ohci->ctrl_head;
            break;

        case 9:  
            retval = ohci->ctrl_cur;
            break;

        case 10:  
            retval = ohci->bulk_head;
            break;

        case 11:  
            retval = ohci->bulk_cur;
            break;

        case 12:  
            retval = ohci->done;
            break;

        case 13:  
            retval = (ohci->fit << 31) | (ohci->fsmps << 16) | (ohci->fi);
            break;

        case 14:  
            retval = ohci_get_frame_remaining(ohci);
            break;

        case 15:  
            retval = ohci->frame_number;
            break;

        case 16:  
            retval = ohci->pstart;
            break;

        case 17:  
            retval = ohci->lst;
            break;

        case 18:  
            retval = ohci->rhdesc_a;
            break;

        case 19:  
            retval = ohci->rhdesc_b;
            break;

        case 20:  
            retval = ohci->rhstatus;
            break;

         
        case 24:  
            retval = ohci->hstatus & ohci->hmask;
            break;

        case 25:  
            retval = ohci->hreset;
            break;

        case 26:  
            retval = ohci->hmask;
            break;

        case 27:  
            retval = ohci->htest;
            break;

        default:
            trace_usb_ohci_mem_read_bad_offset(addr);
            retval = 0xffffffff;
        }
    }

    return retval;
}
