static uint32_t fdctrl_read_dor(FDCtrl *fdctrl)
{
    uint32_t retval = fdctrl->dor;

     
    retval |= fdctrl->cur_drv;
    FLOPPY_DPRINTF("digital output register: 0x%02x\n", retval);

    return retval;
}
