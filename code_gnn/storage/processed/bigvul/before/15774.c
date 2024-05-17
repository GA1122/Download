void ahci_reset(AHCIState *s)
{
    AHCIPortRegs *pr;
    int i;

    s->control_regs.irqstatus = 0;
     
    s->control_regs.ghc = HOST_CTL_AHCI_EN;

    for (i = 0; i < s->ports; i++) {
        pr = &s->dev[i].port_regs;
        pr->irq_stat = 0;
        pr->irq_mask = 0;
        pr->scr_ctl = 0;
        pr->cmd = PORT_CMD_SPIN_UP | PORT_CMD_POWER_ON;
        ahci_reset_port(s, i);
    }
}
