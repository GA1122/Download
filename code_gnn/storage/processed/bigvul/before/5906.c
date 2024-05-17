static int ohci_bus_start(OHCIState *ohci)
{
    trace_usb_ohci_start(ohci->name);

     

    ohci_eof_timer(ohci);

    return 1;
}
