static void ahci_write_fis_d2h(AHCIDevice *ad, uint8_t *cmd_fis)
{
    AHCIPortRegs *pr = &ad->port_regs;
    uint8_t *d2h_fis;
    int i;
    dma_addr_t cmd_len = 0x80;
    int cmd_mapped = 0;

    if (!ad->res_fis || !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    if (!cmd_fis) {
         
        uint64_t tbl_addr = le64_to_cpu(ad->cur_cmd->tbl_addr);
        cmd_fis = dma_memory_map(ad->hba->as, tbl_addr, &cmd_len,
                                 DMA_DIRECTION_TO_DEVICE);
        cmd_mapped = 1;
    }

    d2h_fis = &ad->res_fis[RES_FIS_RFIS];

    d2h_fis[0] = 0x34;
    d2h_fis[1] = (ad->hba->control_regs.irqstatus ? (1 << 6) : 0);
    d2h_fis[2] = ad->port.ifs[0].status;
    d2h_fis[3] = ad->port.ifs[0].error;

    d2h_fis[4] = cmd_fis[4];
    d2h_fis[5] = cmd_fis[5];
    d2h_fis[6] = cmd_fis[6];
    d2h_fis[7] = cmd_fis[7];
    d2h_fis[8] = cmd_fis[8];
    d2h_fis[9] = cmd_fis[9];
    d2h_fis[10] = cmd_fis[10];
    d2h_fis[11] = cmd_fis[11];
    d2h_fis[12] = cmd_fis[12];
    d2h_fis[13] = cmd_fis[13];
    for (i = 14; i < 20; i++) {
        d2h_fis[i] = 0;
    }

    if (d2h_fis[2] & ERR_STAT) {
        ahci_trigger_irq(ad->hba, ad, PORT_IRQ_STAT_TFES);
    }

    ahci_trigger_irq(ad->hba, ad, PORT_IRQ_D2H_REG_FIS);

    if (cmd_mapped) {
        dma_memory_unmap(ad->hba->as, cmd_fis, cmd_len,
                         DMA_DIRECTION_TO_DEVICE, cmd_len);
    }
}
