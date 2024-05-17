void tpm_continue_selftest(struct tpm_chip *chip)
{
	u8 data[] = {
		0, 193,			 
		0, 0, 0, 10,		 
		0, 0, 0, 83,		 
	};

	tpm_transmit(chip, data, sizeof(data));
}
