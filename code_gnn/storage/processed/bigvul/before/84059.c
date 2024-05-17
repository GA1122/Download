static u32 avc_add_emulation_bytes(const char *buffer_src, char *buffer_dst, u32 nal_size)
{
	u32 i = 0, emulation_bytes_count = 0;
	u8 num_zero = 0;

	while (i < nal_size) {
		 
		if (num_zero == 2 && (u8)buffer_src[i] < 0x04) {
			 
			num_zero = 0;
			buffer_dst[i+emulation_bytes_count] = 0x03;
			emulation_bytes_count++;
			if (!buffer_src[i])
				num_zero = 1;
		} else {
			if (!buffer_src[i])
				num_zero++;
			else
				num_zero = 0;
		}
		buffer_dst[i+emulation_bytes_count] = buffer_src[i];
		i++;
	}
	return nal_size+emulation_bytes_count;
}
