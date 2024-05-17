DetachUnboundGPU(ScreenPtr slave)
{
    assert(slave->isGPU);
    assert(!slave->is_output_slave);
    assert(!slave->is_offload_slave);
    xorg_list_del(&slave->slave_head);
    slave->current_master = NULL;
}
