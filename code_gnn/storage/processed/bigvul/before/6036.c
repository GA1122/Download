e1000e_ring_len(E1000ECore *core, const E1000E_RingInfo *r)
{
    return core->mac[r->dlen];
}
