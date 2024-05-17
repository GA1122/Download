e1000e_set_eitr(E1000ECore *core, int index, uint32_t val)
{
    uint32_t interval = val & 0xffff;
    uint32_t eitr_num = index - EITR;

    trace_e1000e_irq_eitr_set(eitr_num, val);

    core->eitr_guest_value[eitr_num] = interval;
    core->mac[index] = MAX(interval, E1000E_MIN_XITR);
}
