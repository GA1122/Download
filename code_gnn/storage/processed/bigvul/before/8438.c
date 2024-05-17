pvscsi_raise_completion_interrupt(PVSCSIState *s)
{
    s->reg_interrupt_status |= PVSCSI_INTR_CMPL_0;

     
    smp_wmb();

    pvscsi_update_irq_status(s);
}