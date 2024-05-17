e1000e_ring_head_descr(E1000ECore *core, const E1000E_RingInfo *r)
{
    return e1000e_ring_base(core, r) + E1000_RING_DESC_LEN * core->mac[r->dh];
}
