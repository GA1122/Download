static unsigned char *get_mcs(int bitrate)
{
	struct yam_mcs *p;

	p = yam_data;
	while (p) {
		if (p->bitrate == bitrate)
			return p->bits;
		p = p->next;
	}

	 
	switch (bitrate) {
	case 1200:
		 
		return add_mcs(NULL, bitrate, YAM_1200);
	default:
		 
		return add_mcs(NULL, bitrate, YAM_9600);
	}
}
