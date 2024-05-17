static int hci_uart_tty_ioctl(struct tty_struct *tty, struct file *file,
			      unsigned int cmd, unsigned long arg)
{
	struct hci_uart *hu = tty->disc_data;
	int err = 0;

	BT_DBG("");

	 
	if (!hu)
		return -EBADF;

	switch (cmd) {
	case HCIUARTSETPROTO:
		if (!test_and_set_bit(HCI_UART_PROTO_SET, &hu->flags)) {
			err = hci_uart_set_proto(hu, arg);
			if (err)
				clear_bit(HCI_UART_PROTO_SET, &hu->flags);
		} else
			err = -EBUSY;
		break;

	case HCIUARTGETPROTO:
		if (test_bit(HCI_UART_PROTO_SET, &hu->flags))
			err = hu->proto->id;
		else
			err = -EUNATCH;
		break;

	case HCIUARTGETDEVICE:
		if (test_bit(HCI_UART_REGISTERED, &hu->flags))
			err = hu->hdev->id;
		else
			err = -EUNATCH;
		break;

	case HCIUARTSETFLAGS:
		if (test_bit(HCI_UART_PROTO_SET, &hu->flags))
			err = -EBUSY;
		else
			err = hci_uart_set_flags(hu, arg);
		break;

	case HCIUARTGETFLAGS:
		err = hu->hdev_flags;
		break;

	default:
		err = n_tty_ioctl_helper(tty, file, cmd, arg);
		break;
	}

	return err;
}