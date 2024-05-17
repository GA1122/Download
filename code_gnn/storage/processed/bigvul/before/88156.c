static int hci_uart_open(struct hci_dev *hdev)
{
	BT_DBG("%s %p", hdev->name, hdev);

	 
	hdev->flush = hci_uart_flush;

	return 0;
}
