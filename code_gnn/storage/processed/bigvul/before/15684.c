static uint32_t openpic_iack(OpenPICState *opp, IRQDest *dst, int cpu)
{
    IRQSource *src;
    int retval, irq;

    DPRINTF("Lower OpenPIC INT output\n");
    qemu_irq_lower(dst->irqs[OPENPIC_OUTPUT_INT]);

    irq = IRQ_get_next(opp, &dst->raised);
    DPRINTF("IACK: irq=%d\n", irq);

    if (irq == -1) {
         
        return opp->spve;
    }

    src = &opp->src[irq];
    if (!(src->ivpr & IVPR_ACTIVITY_MASK) ||
            !(IVPR_PRIORITY(src->ivpr) > dst->ctpr)) {
        fprintf(stderr, "%s: bad raised IRQ %d ctpr %d ivpr 0x%08x\n",
                __func__, irq, dst->ctpr, src->ivpr);
        openpic_update_irq(opp, irq);
        retval = opp->spve;
    } else {
         
        IRQ_setbit(&dst->servicing, irq);
        retval = IVPR_VECTOR(opp, src->ivpr);
    }

    if (!src->level) {
         
        src->ivpr &= ~IVPR_ACTIVITY_MASK;
        src->pending = 0;
        IRQ_resetbit(&dst->raised, irq);
    }

    if ((irq >= opp->irq_ipi0) &&  (irq < (opp->irq_ipi0 + OPENPIC_MAX_IPI))) {
        src->destmask &= ~(1 << cpu);
        if (src->destmask && !src->level) {
             
            openpic_set_irq(opp, irq, 1);
            openpic_set_irq(opp, irq, 0);
             
            src->ivpr |= IVPR_ACTIVITY_MASK;
        }
    }

    return retval;
}
