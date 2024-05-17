static bool cmd_cfa_access_metadata_storage(IDEState *s, uint8_t cmd)
{
    switch (s->feature) {
    case 0x02:   
        ide_cfata_metadata_inquiry(s);
        break;
    case 0x03:   
        ide_cfata_metadata_read(s);
        break;
    case 0x04:   
        ide_cfata_metadata_write(s);
        break;
    default:
        ide_abort_command(s);
        return true;
    }

    ide_transfer_start(s, s->io_buffer, 0x200, ide_transfer_stop);
    s->status = 0x00;  
    ide_set_irq(s->bus);

    return false;
}