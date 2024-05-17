e1000e_read_lgcy_rx_descr(E1000ECore *core, uint8_t *desc, hwaddr *buff_addr)
{
    struct e1000_rx_desc *d = (struct e1000_rx_desc *) desc;
    *buff_addr = le64_to_cpu(d->buffer_addr);
}
