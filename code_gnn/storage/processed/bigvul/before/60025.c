void usb_serial_console_init(int minor)
{
	if (minor == 0) {
		 
		 
		pr_debug("registering the USB serial console.\n");
		register_console(&usbcons);
	}
}
