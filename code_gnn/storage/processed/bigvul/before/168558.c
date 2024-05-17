static int unsupported_clear_halt(int sub_api, struct libusb_device_handle *dev_handle, unsigned char endpoint) {
	PRINT_UNSUPPORTED_API(clear_halt);
}
