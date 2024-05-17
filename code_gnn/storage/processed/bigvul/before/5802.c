static void ehci_port_write(void *ptr, hwaddr addr,
                            uint64_t val, unsigned size)
{
    EHCIState *s = ptr;
    int port = addr >> 2;
    uint32_t *portsc = &s->portsc[port];
    uint32_t old = *portsc;
    USBDevice *dev = s->ports[port].dev;

    trace_usb_ehci_portsc_write(addr + s->portscbase, addr >> 2, val);

     
    *portsc &= ~(val & PORTSC_RWC_MASK);
     
    *portsc &= val | ~PORTSC_PED;
     
    handle_port_owner_write(s, port, val);
     
    val &= PORTSC_RO_MASK;

    if ((val & PORTSC_PRESET) && !(*portsc & PORTSC_PRESET)) {
        trace_usb_ehci_port_reset(port, 1);
    }

    if (!(val & PORTSC_PRESET) &&(*portsc & PORTSC_PRESET)) {
        trace_usb_ehci_port_reset(port, 0);
        if (dev && dev->attached) {
            usb_port_reset(&s->ports[port]);
            *portsc &= ~PORTSC_CSC;
        }

         
        if (dev && dev->attached && (dev->speedmask & USB_SPEED_MASK_HIGH)) {
            val |= PORTSC_PED;
        }
    }

    if ((val & PORTSC_SUSPEND) && !(*portsc & PORTSC_SUSPEND)) {
        trace_usb_ehci_port_suspend(port);
    }
    if (!(val & PORTSC_FPRES) && (*portsc & PORTSC_FPRES)) {
        trace_usb_ehci_port_resume(port);
        val &= ~PORTSC_SUSPEND;
    }

    *portsc &= ~PORTSC_RO_MASK;
    *portsc |= val;
    trace_usb_ehci_portsc_change(addr + s->portscbase, addr >> 2, *portsc, old);
}
