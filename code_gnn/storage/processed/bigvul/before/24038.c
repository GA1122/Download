static int header_len(__le16 ctl)
{
	u16 fc = le16_to_cpu(ctl);
	switch (fc & 0xc) {
	case 4:
		if ((fc & 0xe0) == 0xc0)
			return 10;	 
		return 16;	 
	case 8:
		if ((fc & 0x300) == 0x300)
			return 30;	 
	}
	return 24;
}
