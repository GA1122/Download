int hci_uart_tx_wakeup(struct hci_uart *hu)
{
	 
	if (!percpu_down_read_trylock(&hu->proto_lock))
		return 0;

	if (!test_bit(HCI_UART_PROTO_READY, &hu->flags))
		goto no_schedule;

	if (test_and_set_bit(HCI_UART_SENDING, &hu->tx_state)) {
		set_bit(HCI_UART_TX_WAKEUP, &hu->tx_state);
		goto no_schedule;
	}

	BT_DBG("");

	schedule_work(&hu->write_work);

no_schedule:
	percpu_up_read(&hu->proto_lock);

	return 0;
}
