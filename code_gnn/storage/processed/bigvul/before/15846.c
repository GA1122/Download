static void virtio_net_instance_init(Object *obj)
{
    VirtIONet *n = VIRTIO_NET(obj);

     
    n->config_size = sizeof(struct virtio_net_config);
}
