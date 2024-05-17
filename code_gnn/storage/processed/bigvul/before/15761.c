static void update_irq(struct HPETTimer *timer, int set)
{
    uint64_t mask;
    HPETState *s;
    int route;

    if (timer->tn <= 1 && hpet_in_legacy_mode(timer->state)) {
         
        route = (timer->tn == 0) ? 0 : RTC_ISA_IRQ;
    } else {
        route = timer_int_route(timer);
    }
    s = timer->state;
    mask = 1 << timer->tn;
    if (!set || !timer_enabled(timer) || !hpet_enabled(timer->state)) {
        s->isr &= ~mask;
        if (!timer_fsb_route(timer)) {
             
            if (route >= ISA_NUM_IRQS) {
                qemu_irq_raise(s->irqs[route]);
            } else {
                qemu_irq_lower(s->irqs[route]);
            }
        }
    } else if (timer_fsb_route(timer)) {
        stl_le_phys(&address_space_memory,
                    timer->fsb >> 32, timer->fsb & 0xffffffff);
    } else if (timer->config & HPET_TN_TYPE_LEVEL) {
        s->isr |= mask;
         
        if (route >= ISA_NUM_IRQS) {
            qemu_irq_lower(s->irqs[route]);
        } else {
            qemu_irq_raise(s->irqs[route]);
        }
    } else {
        s->isr &= ~mask;
        qemu_irq_pulse(s->irqs[route]);
    }
}
