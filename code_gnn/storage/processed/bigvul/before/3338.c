static void fdctrl_reset_irq(FDCtrl *fdctrl)
{
    fdctrl->status0 = 0;
    if (!(fdctrl->sra & FD_SRA_INTPEND))
        return;
    FLOPPY_DPRINTF("Reset interrupt\n");
    qemu_set_irq(fdctrl->irq, 0);
    fdctrl->sra &= ~FD_SRA_INTPEND;
}
