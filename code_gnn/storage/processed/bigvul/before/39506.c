fst_attach(struct net_device *dev, unsigned short encoding, unsigned short parity)
{
	 
	if (encoding != ENCODING_NRZ || parity != PARITY_CRC16_PR1_CCITT)
		return -EINVAL;
	return 0;
}
