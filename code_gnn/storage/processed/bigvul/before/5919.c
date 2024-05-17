static void ohci_mem_write(void *opaque,
                           hwaddr addr,
                           uint64_t val,
                           unsigned size)
{
    OHCIState *ohci = opaque;

     
    if (addr & 3) {
        trace_usb_ohci_mem_write_unaligned(addr);
        return;
    }

    if (addr >= 0x54 && addr < 0x54 + ohci->num_ports * 4) {
         
        ohci_port_set_status(ohci, (addr - 0x54) >> 2, val);
        return;
    }

    switch (addr >> 2) {
    case 1:  
        ohci_set_ctl(ohci, val);
        break;

    case 2:  
         
        val = (val & ~OHCI_STATUS_SOC);

         
        ohci->status |= val;

        if (ohci->status & OHCI_STATUS_HCR)
            ohci_soft_reset(ohci);
        break;

    case 3:  
        ohci->intr_status &= ~val;
        ohci_intr_update(ohci);
        break;

    case 4:  
        ohci->intr |= val;
        ohci_intr_update(ohci);
        break;

    case 5:  
        ohci->intr &= ~val;
        ohci_intr_update(ohci);
        break;

    case 6:  
        ohci->hcca = val & OHCI_HCCA_MASK;
        break;

    case 7:  
         
        break;

    case 8:  
        ohci->ctrl_head = val & OHCI_EDPTR_MASK;
        break;

    case 9:  
        ohci->ctrl_cur = val & OHCI_EDPTR_MASK;
        break;

    case 10:  
        ohci->bulk_head = val & OHCI_EDPTR_MASK;
        break;

    case 11:  
        ohci->bulk_cur = val & OHCI_EDPTR_MASK;
        break;

    case 13:  
        ohci->fsmps = (val & OHCI_FMI_FSMPS) >> 16;
        ohci->fit = (val & OHCI_FMI_FIT) >> 31;
        ohci_set_frame_interval(ohci, val);
        break;

    case 15:  
        break;

    case 16:  
        ohci->pstart = val & 0xffff;
        break;

    case 17:  
        ohci->lst = val & 0xffff;
        break;

    case 18:  
        ohci->rhdesc_a &= ~OHCI_RHA_RW_MASK;
        ohci->rhdesc_a |= val & OHCI_RHA_RW_MASK;
        break;

    case 19:  
        break;

    case 20:  
        ohci_set_hub_status(ohci, val);
        break;

     
    case 24:  
        ohci->hstatus &= ~(val & ohci->hmask);
        break;

    case 25:  
        ohci->hreset = val & ~OHCI_HRESET_FSBIR;
        if (val & OHCI_HRESET_FSBIR)
            ohci_hard_reset(ohci);
        break;

    case 26:  
        ohci->hmask = val;
        break;

    case 27:  
        ohci->htest = val;
        break;

    default:
        trace_usb_ohci_mem_write_bad_offset(addr);
        break;
    }
}
