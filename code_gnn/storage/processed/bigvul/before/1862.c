uint8_t *reds_get_agent_data_buffer(MainChannelClient *mcc, size_t size)
{
    VDIPortState *dev_state = &reds->agent_state;
    RedClient *client;

    if (!dev_state->client_agent_started) {
         
        return spice_malloc(size);
    }

    spice_assert(dev_state->recv_from_client_buf == NULL);
    client = main_channel_client_get_base(mcc)->client;
    dev_state->recv_from_client_buf = spice_char_device_write_buffer_get(dev_state->base,
                                                                         client,
                                                                         size + sizeof(VDIChunkHeader));
    dev_state->recv_from_client_buf_pushed = FALSE;
    return dev_state->recv_from_client_buf->buf + sizeof(VDIChunkHeader);
}
