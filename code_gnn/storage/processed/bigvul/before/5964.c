e1000e_core_post_load(E1000ECore *core)
{
    NetClientState *nc = qemu_get_queue(core->owner_nic);

     
    nc->link_down = (core->mac[STATUS] & E1000_STATUS_LU) == 0;

    return 0;
}
