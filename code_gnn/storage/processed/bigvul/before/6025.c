e1000e_read_ext_rx_descr(E1000ECore *core, uint8_t *desc, hwaddr *buff_addr)
{
    union e1000_rx_desc_extended *d = (union e1000_rx_desc_extended *) desc;
    *buff_addr = le64_to_cpu(d->read.buffer_addr);
}
