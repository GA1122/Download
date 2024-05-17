static void ehci_advance_async_state(EHCIState *ehci)
{
    const int async = 1;

    switch(ehci_get_state(ehci, async)) {
    case EST_INACTIVE:
        if (!ehci_async_enabled(ehci)) {
            break;
        }
        ehci_set_state(ehci, async, EST_ACTIVE);

    case EST_ACTIVE:
        if (!ehci_async_enabled(ehci)) {
            ehci_queues_rip_all(ehci, async);
            ehci_set_state(ehci, async, EST_INACTIVE);
            break;
        }

         
         
        if (ehci->usbsts & USBSTS_IAA) {
            DPRINTF("IAA status bit still set.\n");
            break;
        }

         
        if (ehci->asynclistaddr == 0) {
            break;
        }

        ehci_set_state(ehci, async, EST_WAITLISTHEAD);
        ehci_advance_state(ehci, async);

         
        if (ehci->usbcmd & USBCMD_IAAD) {
             
            ehci_queues_rip_unseen(ehci, async);
            trace_usb_ehci_doorbell_ack();
            ehci->usbcmd &= ~USBCMD_IAAD;
            ehci_raise_irq(ehci, USBSTS_IAA);
        }
        break;

    default:
         
        fprintf(stderr, "ehci: Bad asynchronous state %d. "
                "Resetting to active\n", ehci->astate);
        g_assert_not_reached();
    }
}