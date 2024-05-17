static void resize_console(struct port *port)
{
	struct virtio_device *vdev;

	 
	if (!port || !is_console_port(port))
		return;

	vdev = port->portdev->vdev;

	 
	if (!is_rproc_serial(vdev) &&
	    virtio_has_feature(vdev, VIRTIO_CONSOLE_F_SIZE))
		hvc_resize(port->cons.hvc, port->cons.ws);
}
