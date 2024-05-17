e1000e_ring_enabled(E1000ECore *core, const E1000E_RingInfo *r)
{
    return core->mac[r->dlen] > 0;
}
