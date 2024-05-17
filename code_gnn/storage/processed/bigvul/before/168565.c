static int unsupported_release_interface(int sub_api, struct libusb_device_handle *dev_handle, int iface) {
	PRINT_UNSUPPORTED_API(release_interface);
}
