static bool cmd_cfa_erase_sectors(IDEState *s, uint8_t cmd)
{
     

    if (cmd == CFA_WEAR_LEVEL) {
        s->nsector = 0;
    }

    if (cmd == CFA_ERASE_SECTORS) {
        s->media_changed = 1;
    }

    return true;
}
