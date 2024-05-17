static int download_code(struct edgeport_serial *serial, __u8 *image,
							int image_length)
{
	int status = 0;
	int pos;
	int transfer;
	int done;

	 
	for (pos = 0; pos < image_length; ) {
		 
		transfer = image_length - pos;
		if (transfer > EDGE_FW_BULK_MAX_PACKET_SIZE)
			transfer = EDGE_FW_BULK_MAX_PACKET_SIZE;

		 
		status = bulk_xfer(serial->serial, &image[pos],
							transfer, &done);
		if (status)
			break;
		 
		pos += done;
	}

	return status;
}
