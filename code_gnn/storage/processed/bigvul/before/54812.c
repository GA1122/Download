static int snd_usbmidi_urb_error(const struct urb *urb)
{
	switch (urb->status) {
	 
	case -ENOENT:
	case -ECONNRESET:
	case -ESHUTDOWN:
	case -ENODEV:
		return -ENODEV;
	 
	case -EPROTO:
	case -ETIME:
	case -EILSEQ:
		return -EIO;
	default:
		dev_err(&urb->dev->dev, "urb status %d\n", urb->status);
		return 0;  
	}
}
