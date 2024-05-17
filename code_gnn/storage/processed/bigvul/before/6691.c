static bool cmd_exec_dev_diagnostic(IDEState *s, uint8_t cmd)
{
    ide_set_signature(s);

    if (s->drive_kind == IDE_CD) {
        s->status = 0;  
        s->error = 0x01;
    } else {
        s->status = READY_STAT | SEEK_STAT;
         
        s->error = 0x01;
        ide_set_irq(s->bus);
    }

    return false;
}
