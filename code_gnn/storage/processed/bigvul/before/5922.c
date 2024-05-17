static int ohci_port_set_if_connected(OHCIState *ohci, int i, uint32_t val)
{
    int ret = 1;

     
    if (val == 0)
        return 0;

     
    if (!(ohci->rhport[i].ctrl & OHCI_PORT_CCS)) {
        ohci->rhport[i].ctrl |= OHCI_PORT_CSC;
        if (ohci->rhstatus & OHCI_RHS_DRWE) {
             
        }
        return 0;
    }

    if (ohci->rhport[i].ctrl & val)
        ret = 0;

     
    ohci->rhport[i].ctrl |= val;

    return ret;
}
