static SpiceCharDeviceMsgToClient *vdi_port_ref_msg_to_client(SpiceCharDeviceMsgToClient *msg,
                                                       void *opaque)
{
    return vdi_port_read_buf_ref(msg);
}
