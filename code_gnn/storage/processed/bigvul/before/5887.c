static void ahci_write_fis_sdb(AHCIState *s, NCQTransferState *ncq_tfs)
{
    AHCIDevice *ad = ncq_tfs->drive;
    AHCIPortRegs *pr = &ad->port_regs;
    IDEState *ide_state;
    SDBFIS *sdb_fis;

    if (!ad->res_fis ||
        !(pr->cmd & PORT_CMD_FIS_RX)) {
        return;
    }

    sdb_fis = (SDBFIS *)&ad->res_fis[RES_FIS_SDBFIS];
    ide_state = &ad->port.ifs[0];

    sdb_fis->type = SATA_FIS_TYPE_SDB;
     
    sdb_fis->flags = 0x40;  
    sdb_fis->status = ide_state->status & 0x77;
    sdb_fis->error = ide_state->error;
     
    sdb_fis->payload = cpu_to_le32(ad->finished);

     
    pr->tfdata = (ad->port.ifs[0].error << 8) |
        (ad->port.ifs[0].status & 0x77) |
        (pr->tfdata & 0x88);
    pr->scr_act &= ~ad->finished;
    ad->finished = 0;

     
    if (sdb_fis->flags & 0x40) {
        ahci_trigger_irq(s, ad, PORT_IRQ_SDB_FIS);
    }
}
