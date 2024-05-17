static int handle_cmd(AHCIState *s, int port, int slot)
{
    IDEState *ide_state;
    uint32_t opts;
    uint64_t tbl_addr;
    AHCICmdHdr *cmd;
    uint8_t *cmd_fis;
    dma_addr_t cmd_len;

    if (s->dev[port].port.ifs[0].status & (BUSY_STAT|DRQ_STAT)) {
         
        DPRINTF(port, "engine busy\n");
        return -1;
    }

    cmd = &((AHCICmdHdr *)s->dev[port].lst)[slot];

    if (!s->dev[port].lst) {
        DPRINTF(port, "error: lst not given but cmd handled");
        return -1;
    }

     
    s->dev[port].cur_cmd = cmd;

    opts = le32_to_cpu(cmd->opts);
    tbl_addr = le64_to_cpu(cmd->tbl_addr);

    cmd_len = 0x80;
    cmd_fis = dma_memory_map(s->as, tbl_addr, &cmd_len,
                             DMA_DIRECTION_FROM_DEVICE);

    if (!cmd_fis) {
        DPRINTF(port, "error: guest passed us an invalid cmd fis\n");
        return -1;
    }

     
    ide_state = &s->dev[port].port.ifs[0];

    if (!ide_state->bs) {
        DPRINTF(port, "error: guest accessed unused port");
        goto out;
    }

    debug_print_fis(cmd_fis, 0x90);

    switch (cmd_fis[0]) {
        case SATA_FIS_TYPE_REGISTER_H2D:
            break;
        default:
            DPRINTF(port, "unknown command cmd_fis[0]=%02x cmd_fis[1]=%02x "
                          "cmd_fis[2]=%02x\n", cmd_fis[0], cmd_fis[1],
                          cmd_fis[2]);
            goto out;
            break;
    }

    switch (cmd_fis[1]) {
        case SATA_FIS_REG_H2D_UPDATE_COMMAND_REGISTER:
            break;
        case 0:
            break;
        default:
            DPRINTF(port, "unknown command cmd_fis[0]=%02x cmd_fis[1]=%02x "
                          "cmd_fis[2]=%02x\n", cmd_fis[0], cmd_fis[1],
                          cmd_fis[2]);
            goto out;
            break;
    }

    switch (s->dev[port].port_state) {
        case STATE_RUN:
            if (cmd_fis[15] & ATA_SRST) {
                s->dev[port].port_state = STATE_RESET;
            }
            break;
        case STATE_RESET:
            if (!(cmd_fis[15] & ATA_SRST)) {
                ahci_reset_port(s, port);
            }
            break;
    }

    if (cmd_fis[1] == SATA_FIS_REG_H2D_UPDATE_COMMAND_REGISTER) {

         
        if ((cmd_fis[2] == READ_FPDMA_QUEUED) ||
            (cmd_fis[2] == WRITE_FPDMA_QUEUED)) {
            process_ncq_command(s, port, cmd_fis, slot);
            goto out;
        }

         
        ide_state->nsector = (int64_t)((cmd_fis[13] << 8) | cmd_fis[12]);
        ide_state->feature = cmd_fis[3];
        if (!ide_state->nsector) {
            ide_state->nsector = 256;
        }

        if (ide_state->drive_kind != IDE_CD) {
             
            ide_set_sector(ide_state, ((uint64_t)cmd_fis[10] << 40)
                                    | ((uint64_t)cmd_fis[9] << 32)
                                     
                                    | ((uint64_t)cmd_fis[8] << 24)
                                     
                                    | ((uint64_t)(cmd_fis[7] & 0xf) << 24)
                                    | ((uint64_t)cmd_fis[6] << 16)
                                    | ((uint64_t)cmd_fis[5] << 8)
                                    | cmd_fis[4]);
        }

         
        if (opts & AHCI_CMD_ATAPI) {
            memcpy(ide_state->io_buffer, &cmd_fis[AHCI_COMMAND_TABLE_ACMD], 0x10);
            ide_state->lcyl = 0x14;
            ide_state->hcyl = 0xeb;
            debug_print_fis(ide_state->io_buffer, 0x10);
            ide_state->feature = IDE_FEATURE_DMA;
            s->dev[port].done_atapi_packet = false;
             
        }

        ide_state->error = 0;

         
        cmd->status = 0;

         
        ide_exec_cmd(&s->dev[port].port, cmd_fis[2]);

        if ((s->dev[port].port.ifs[0].status & (READY_STAT|DRQ_STAT|BUSY_STAT)) ==
            READY_STAT) {
            ahci_write_fis_d2h(&s->dev[port], cmd_fis);
        }
    }

out:
    dma_memory_unmap(s->as, cmd_fis, cmd_len, DMA_DIRECTION_FROM_DEVICE,
                     cmd_len);

    if (s->dev[port].port.ifs[0].status & (BUSY_STAT|DRQ_STAT)) {
         
        s->dev[port].busy_slot = slot;
        return -1;
    }

     
    return 0;
}