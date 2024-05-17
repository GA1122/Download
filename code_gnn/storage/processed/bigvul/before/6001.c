e1000e_mac_eitr_read(E1000ECore *core, int index)
{
    return core->eitr_guest_value[index - EITR];
}
