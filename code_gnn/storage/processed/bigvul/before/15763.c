static int ahci_dma_add_status(IDEDMA *dma, int status)
{
    AHCIDevice *ad = DO_UPCAST(AHCIDevice, dma, dma);
    DPRINTF(ad->port_no, "set status: %x\n", status);

    if (status & BM_STATUS_INT) {
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_STAT_DSS);
    }

    return 0;
}
