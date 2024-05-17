static uint32_t fdctrl_read_dir(FDCtrl *fdctrl)
{
    uint32_t retval = 0;

    if (fdctrl_media_changed(get_cur_drv(fdctrl))) {
        retval |= FD_DIR_DSKCHG;
    }
    if (retval != 0) {
        FLOPPY_DPRINTF("Floppy digital input register: 0x%02x\n", retval);
    }

    return retval;
}
