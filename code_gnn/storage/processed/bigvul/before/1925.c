static void reds_reset_vdp(void)
{
    VDIPortState *state = &reds->agent_state;
    SpiceCharDeviceInterface *sif;

    state->read_state = VDI_PORT_READ_STATE_READ_HEADER;
    state->receive_pos = (uint8_t *)&state->vdi_chunk_header;
    state->receive_len = sizeof(state->vdi_chunk_header);
    state->message_receive_len = 0;
    if (state->current_read_buf) {
        vdi_port_read_buf_unref(state->current_read_buf);
        state->current_read_buf = NULL;
    }
     
    agent_msg_filter_init(&state->read_filter, agent_copypaste,
                          agent_file_xfer, TRUE);
     
    state->write_filter.result = AGENT_MSG_FILTER_DISCARD;
    state->write_filter.discard_all = TRUE;
    state->client_agent_started = FALSE;

     
    if (red_channel_test_remote_cap(&reds->main_channel->base,
                                    SPICE_MAIN_CAP_AGENT_CONNECTED_TOKENS)) {
        spice_char_device_state_destroy(state->base);
        state->base = NULL;
    } else {
        spice_char_device_reset(state->base);
    }

    sif = SPICE_CONTAINEROF(vdagent->base.sif, SpiceCharDeviceInterface, base);
    if (sif->state) {
        sif->state(vdagent, 0);
    }
}
