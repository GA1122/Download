static void openpic_cpu_write_internal(void *opaque, hwaddr addr,
                                       uint32_t val, int idx)
{
    OpenPICState *opp = opaque;
    IRQSource *src;
    IRQDest *dst;
    int s_IRQ, n_IRQ;

    DPRINTF("%s: cpu %d addr %#" HWADDR_PRIx " <= 0x%08x\n", __func__, idx,
            addr, val);

    if (idx < 0) {
        return;
    }

    if (addr & 0xF) {
        return;
    }
    dst = &opp->dst[idx];
    addr &= 0xFF0;
    switch (addr) {
    case 0x40:  
    case 0x50:
    case 0x60:
    case 0x70:
        idx = (addr - 0x40) >> 4;
         
        opp->src[opp->irq_ipi0 + idx].destmask |= val;
        openpic_set_irq(opp, opp->irq_ipi0 + idx, 1);
        openpic_set_irq(opp, opp->irq_ipi0 + idx, 0);
        break;
    case 0x80:  
        dst->ctpr = val & 0x0000000F;

        DPRINTF("%s: set CPU %d ctpr to %d, raised %d servicing %d\n",
                __func__, idx, dst->ctpr, dst->raised.priority,
                dst->servicing.priority);

        if (dst->raised.priority <= dst->ctpr) {
            DPRINTF("%s: Lower OpenPIC INT output cpu %d due to ctpr\n",
                    __func__, idx);
            qemu_irq_lower(dst->irqs[OPENPIC_OUTPUT_INT]);
        } else if (dst->raised.priority > dst->servicing.priority) {
            DPRINTF("%s: Raise OpenPIC INT output cpu %d irq %d\n",
                    __func__, idx, dst->raised.next);
            qemu_irq_raise(dst->irqs[OPENPIC_OUTPUT_INT]);
        }

        break;
    case 0x90:  
         
        break;
    case 0xA0:  
         
        break;
    case 0xB0:  
        DPRINTF("EOI\n");
        s_IRQ = IRQ_get_next(opp, &dst->servicing);

        if (s_IRQ < 0) {
            DPRINTF("%s: EOI with no interrupt in service\n", __func__);
            break;
        }

        IRQ_resetbit(&dst->servicing, s_IRQ);
         
        s_IRQ = IRQ_get_next(opp, &dst->servicing);
         
        n_IRQ = IRQ_get_next(opp, &dst->raised);
        src = &opp->src[n_IRQ];
        if (n_IRQ != -1 &&
            (s_IRQ == -1 ||
             IVPR_PRIORITY(src->ivpr) > dst->servicing.priority)) {
            DPRINTF("Raise OpenPIC INT output cpu %d irq %d\n",
                    idx, n_IRQ);
            qemu_irq_raise(opp->dst[idx].irqs[OPENPIC_OUTPUT_INT]);
        }
        break;
    default:
        break;
    }
}
