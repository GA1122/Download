static void handle_reg_h2d_fis(AHCIState *s, int port,
                               uint8_t slot, uint8_t *cmd_fis)
{
    IDEState *ide_state = &s->dev[port].port.ifs[0];
    AHCICmdHdr *cmd = get_cmd_header(s, port, slot);
    uint16_t opts = le16_to_cpu(cmd->opts);

    if (cmd_fis[1] & 0x0F) {
        DPRINTF(port, "Port Multiplier not supported."
                " cmd_fis[0]=%02x cmd_fis[1]=%02x cmd_fis[2]=%02x\n",
                cmd_fis[0], cmd_fis[1], cmd_fis[2]);
        return;
    }

    if (cmd_fis[1] & 0x70) {
        DPRINTF(port, "Reserved flags set in H2D Register FIS."
                " cmd_fis[0]=%02x cmd_fis[1]=%02x cmd_fis[2]=%02x\n",
                cmd_fis[0], cmd_fis[1], cmd_fis[2]);
        return;
    }

    if (!(cmd_fis[1] & SATA_FIS_REG_H2D_UPDATE_COMMAND_REGISTER)) {
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
        return;
    }

     
    if (is_ncq(cmd_fis[2])) {
        process_ncq_command(s, port, cmd_fis, slot);
        return;
    }

     
    ide_state->feature = cmd_fis[3];
    ide_state->sector = cmd_fis[4];       
    ide_state->lcyl = cmd_fis[5];         
    ide_state->hcyl = cmd_fis[6];         
    ide_state->select = cmd_fis[7];       
    ide_state->hob_sector = cmd_fis[8];   
    ide_state->hob_lcyl = cmd_fis[9];     
    ide_state->hob_hcyl = cmd_fis[10];    
    ide_state->hob_feature = cmd_fis[11];
    ide_state->nsector = (int64_t)((cmd_fis[13] << 8) | cmd_fis[12]);
     
     

     
    if (opts & AHCI_CMD_ATAPI) {
        memcpy(ide_state->io_buffer, &cmd_fis[AHCI_COMMAND_TABLE_ACMD], 0x10);
        debug_print_fis(ide_state->io_buffer, 0x10);
        s->dev[port].done_atapi_packet = false;
         
    }

    ide_state->error = 0;

     
    cmd->status = 0;

     
    ide_exec_cmd(&s->dev[port].port, cmd_fis[2]);
}
