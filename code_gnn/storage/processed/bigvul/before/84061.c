static u32 avc_emulation_bytes_add_count(char *buffer, u32 nal_size)
{
	u32 i = 0, emulation_bytes_count = 0;
	u8 num_zero = 0;

	while (i < nal_size) {
		 
		if (num_zero == 2 && buffer[i] < 0x04) {
			 
			num_zero = 0;
			emulation_bytes_count++;
			if (!buffer[i])
				num_zero = 1;
		} else {
			if (!buffer[i])
				num_zero++;
			else
				num_zero = 0;
		}
		i++;
	}
	return emulation_bytes_count;
}
