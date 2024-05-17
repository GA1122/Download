static void usb_net_handle_destroy(USBDevice *dev)
{
    USBNetState *s = (USBNetState *) dev;

     
    rndis_clear_responsequeue(s);
    qemu_del_nic(s->nic);
}
