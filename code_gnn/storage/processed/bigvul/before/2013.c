static void vdi_port_read_buf_unref(VDIReadBuf *buf)
{
    if (!--buf->refs) {
        ring_add(&reds->agent_state.read_bufs, &buf->link);

         
        if (reds->agent_state.base) {
            spice_char_device_wakeup(reds->agent_state.base);
        }
    }
}
