static int ahci_async_cmd_done(IDEDMA *dma)
{
    AHCIDevice *ad = DO_UPCAST(AHCIDevice, dma, dma);

    DPRINTF(ad->port_no, "async cmd done\n");

     
    ahci_write_fis_d2h(ad, NULL);

    if (!ad->check_bh) {
         
        ad->check_bh = qemu_bh_new(ahci_check_cmd_bh, ad);
        qemu_bh_schedule(ad->check_bh);
    }

    return 0;
}
