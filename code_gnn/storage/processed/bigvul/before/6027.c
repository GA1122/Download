e1000e_read_ps_rx_descr(E1000ECore *core, uint8_t *desc,
                        hwaddr (*buff_addr)[MAX_PS_BUFFERS])
{
    int i;
    union e1000_rx_desc_packet_split *d =
        (union e1000_rx_desc_packet_split *) desc;

    for (i = 0; i < MAX_PS_BUFFERS; i++) {
        (*buff_addr)[i] = le64_to_cpu(d->read.buffer_addr[i]);
    }

    trace_e1000e_rx_desc_ps_read((*buff_addr)[0], (*buff_addr)[1],
                                 (*buff_addr)[2], (*buff_addr)[3]);
}
