static void fdctrl_write_ccr(FDCtrl *fdctrl, uint32_t value)
{
     
    if (!(fdctrl->dor & FD_DOR_nRESET)) {
        FLOPPY_DPRINTF("Floppy controller in RESET state !\n");
        return;
    }
    FLOPPY_DPRINTF("configuration control register set to 0x%02x\n", value);

     
    fdctrl->dsr = (fdctrl->dsr & ~FD_DSR_DRATEMASK) |
                  (value & FD_DSR_DRATEMASK);
}
