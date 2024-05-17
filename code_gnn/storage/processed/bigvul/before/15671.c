static void IRQ_local_pipe(OpenPICState *opp, int n_CPU, int n_IRQ,
                           bool active, bool was_active)
{
    IRQDest *dst;
    IRQSource *src;
    int priority;

    dst = &opp->dst[n_CPU];
    src = &opp->src[n_IRQ];

    DPRINTF("%s: IRQ %d active %d was %d\n",
            __func__, n_IRQ, active, was_active);

    if (src->output != OPENPIC_OUTPUT_INT) {
        DPRINTF("%s: output %d irq %d active %d was %d count %d\n",
                __func__, src->output, n_IRQ, active, was_active,
                dst->outputs_active[src->output]);

         
        if (active) {
            if (!was_active && dst->outputs_active[src->output]++ == 0) {
                DPRINTF("%s: Raise OpenPIC output %d cpu %d irq %d\n",
                        __func__, src->output, n_CPU, n_IRQ);
                qemu_irq_raise(dst->irqs[src->output]);
            }
        } else {
            if (was_active && --dst->outputs_active[src->output] == 0) {
                DPRINTF("%s: Lower OpenPIC output %d cpu %d irq %d\n",
                        __func__, src->output, n_CPU, n_IRQ);
                qemu_irq_lower(dst->irqs[src->output]);
            }
        }

        return;
    }

    priority = IVPR_PRIORITY(src->ivpr);

     
    if (active) {
        IRQ_setbit(&dst->raised, n_IRQ);
    } else {
        IRQ_resetbit(&dst->raised, n_IRQ);
    }

    IRQ_check(opp, &dst->raised);

    if (active && priority <= dst->ctpr) {
        DPRINTF("%s: IRQ %d priority %d too low for ctpr %d on CPU %d\n",
                __func__, n_IRQ, priority, dst->ctpr, n_CPU);
        active = 0;
    }

    if (active) {
        if (IRQ_get_next(opp, &dst->servicing) >= 0 &&
                priority <= dst->servicing.priority) {
            DPRINTF("%s: IRQ %d is hidden by servicing IRQ %d on CPU %d\n",
                    __func__, n_IRQ, dst->servicing.next, n_CPU);
        } else {
            DPRINTF("%s: Raise OpenPIC INT output cpu %d irq %d/%d\n",
                    __func__, n_CPU, n_IRQ, dst->raised.next);
            qemu_irq_raise(opp->dst[n_CPU].irqs[OPENPIC_OUTPUT_INT]);
        }
    } else {
        IRQ_get_next(opp, &dst->servicing);
        if (dst->raised.priority > dst->ctpr &&
                dst->raised.priority > dst->servicing.priority) {
            DPRINTF("%s: IRQ %d inactive, IRQ %d prio %d above %d/%d, CPU %d\n",
                    __func__, n_IRQ, dst->raised.next, dst->raised.priority,
                    dst->ctpr, dst->servicing.priority, n_CPU);
             
        } else {
            DPRINTF("%s: IRQ %d inactive, current prio %d/%d, CPU %d\n",
                    __func__, n_IRQ, dst->ctpr, dst->servicing.priority, n_CPU);
            qemu_irq_lower(opp->dst[n_CPU].irqs[OPENPIC_OUTPUT_INT]);
        }
    }
}
