static void fdctrl_raise_irq(FDCtrl *fdctrl)
{
    if (!(fdctrl->sra & FD_SRA_INTPEND)) {
        qemu_set_irq(fdctrl->irq, 1);
        fdctrl->sra |= FD_SRA_INTPEND;
    }

    fdctrl->reset_sensei = 0;
    FLOPPY_DPRINTF("Set interrupt status to 0x%02x\n", fdctrl->status0);
}
