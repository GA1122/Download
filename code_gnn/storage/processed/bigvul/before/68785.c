static void atusb_tx_done(struct atusb *atusb, uint8_t seq)
{
	struct usb_device *usb_dev = atusb->usb_dev;
	uint8_t expect = atusb->tx_ack_seq;

	dev_dbg(&usb_dev->dev, "atusb_tx_done (0x%02x/0x%02x)\n", seq, expect);
	if (seq == expect) {
		 
		ieee802154_xmit_complete(atusb->hw, atusb->tx_skb, false);
	} else {
		 
		ieee802154_wake_queue(atusb->hw);
		if (atusb->tx_skb)
			dev_kfree_skb_irq(atusb->tx_skb);
	}
}
