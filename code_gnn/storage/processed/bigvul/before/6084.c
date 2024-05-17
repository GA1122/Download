e1000e_set_vet(E1000ECore *core, int index, uint32_t val)
{
    core->mac[VET] = val & 0xffff;
    core->vet = le16_to_cpu(core->mac[VET]);
    trace_e1000e_vlan_vet(core->vet);
}
