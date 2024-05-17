e1000e_read_rx_descr(E1000ECore *core, uint8_t *desc,
                     hwaddr (*buff_addr)[MAX_PS_BUFFERS])
{
    if (e1000e_rx_use_legacy_descriptor(core)) {
        e1000e_read_lgcy_rx_descr(core, desc, &(*buff_addr)[0]);
        (*buff_addr)[1] = (*buff_addr)[2] = (*buff_addr)[3] = 0;
    } else {
        if (core->mac[RCTL] & E1000_RCTL_DTYP_PS) {
            e1000e_read_ps_rx_descr(core, desc, buff_addr);
        } else {
            e1000e_read_ext_rx_descr(core, desc, &(*buff_addr)[0]);
            (*buff_addr)[1] = (*buff_addr)[2] = (*buff_addr)[3] = 0;
        }
    }
}
