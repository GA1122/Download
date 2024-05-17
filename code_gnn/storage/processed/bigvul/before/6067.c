e1000e_set_itr(E1000ECore *core, int index, uint32_t val)
{
    uint32_t interval = val & 0xffff;

    trace_e1000e_irq_itr_set(val);

    core->itr_guest_value = interval;
    core->mac[index] = MAX(interval, E1000E_MIN_XITR);
}
