e1000e_write_hdr_to_rx_buffers(E1000ECore *core,
                               hwaddr (*ba)[MAX_PS_BUFFERS],
                               e1000e_ba_state *bastate,
                               const char *data,
                               dma_addr_t data_len)
{
    assert(data_len <= core->rxbuf_sizes[0] - bastate->written[0]);

    pci_dma_write(core->owner, (*ba)[0] + bastate->written[0], data, data_len);
    bastate->written[0] += data_len;

    bastate->cur_idx = 1;
}
