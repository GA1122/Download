static void windows_destroy_device(struct libusb_device *dev)
{
	windows_device_priv_release(dev);
}
