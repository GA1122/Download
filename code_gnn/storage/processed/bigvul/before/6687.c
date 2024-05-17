static bool cmd_cfa_translate_sector(IDEState *s, uint8_t cmd)
{
    s->status = READY_STAT | SEEK_STAT;

    memset(s->io_buffer, 0, 0x200);
    s->io_buffer[0x00] = s->hcyl;                    
    s->io_buffer[0x01] = s->lcyl;                    
    s->io_buffer[0x02] = s->select;                  
    s->io_buffer[0x03] = s->sector;                  
    s->io_buffer[0x04] = ide_get_sector(s) >> 16;    
    s->io_buffer[0x05] = ide_get_sector(s) >> 8;     
    s->io_buffer[0x06] = ide_get_sector(s) >> 0;     
    s->io_buffer[0x13] = 0x00;                       
    s->io_buffer[0x18] = 0x00;                       
    s->io_buffer[0x19] = 0x00;                       
    s->io_buffer[0x1a] = 0x01;                       

    ide_transfer_start(s, s->io_buffer, 0x200, ide_transfer_stop);
    ide_set_irq(s->bus);

    return false;
}
