rdpsnd_process_negotiate(STREAM in)
{
	uint16 in_format_count, i;
	uint8 pad;
	uint16 version;
	RD_WAVEFORMATEX *format;
	STREAM out;
	RD_BOOL device_available = False;
	int readcnt;
	int discardcnt;

	in_uint8s(in, 14);	 
	in_uint16_le(in, in_format_count);
	in_uint8(in, pad);
	in_uint16_le(in, version);
	in_uint8s(in, 1);	 

	logger(Sound, Debug,
	       "rdpsnd_process_negotiate(), formats = %d, pad = 0x%02x, version = 0x%x",
	       (int) in_format_count, (unsigned) pad, (unsigned) version);

	if (rdpsnd_negotiated)
	{
		 
		rdpsnd_reset_state();
	}

	if (!current_driver && g_rdpsnd)
		device_available = rdpsnd_auto_select();

	if (current_driver && !device_available && current_driver->wave_out_open())
	{
		current_driver->wave_out_close();
		device_available = True;
	}

	format_count = 0;
	if (s_check_rem(in, 18 * in_format_count))
	{
		for (i = 0; i < in_format_count; i++)
		{
			format = &formats[format_count];
			in_uint16_le(in, format->wFormatTag);
			in_uint16_le(in, format->nChannels);
			in_uint32_le(in, format->nSamplesPerSec);
			in_uint32_le(in, format->nAvgBytesPerSec);
			in_uint16_le(in, format->nBlockAlign);
			in_uint16_le(in, format->wBitsPerSample);
			in_uint16_le(in, format->cbSize);

			 
			readcnt = format->cbSize;
			discardcnt = 0;
			if (format->cbSize > MAX_CBSIZE)
			{
				logger(Sound, Debug,
				       "rdpsnd_process_negotiate(), cbSize too large for buffer: %d",
				       format->cbSize);
				readcnt = MAX_CBSIZE;
				discardcnt = format->cbSize - MAX_CBSIZE;
			}
			in_uint8a(in, format->cb, readcnt);
			in_uint8s(in, discardcnt);

			if (current_driver && current_driver->wave_out_format_supported(format))
			{
				format_count++;
				if (format_count == MAX_FORMATS)
					break;
			}
		}
	}

	out = rdpsnd_init_packet(SNDC_FORMATS, 20 + 18 * format_count);

	uint32 flags = TSSNDCAPS_VOLUME;

	 
	if (g_rdpsnd)
	{
		flags |= TSSNDCAPS_ALIVE;
	}
	out_uint32_le(out, flags);	 

	out_uint32(out, 0xffffffff);	 
	out_uint32(out, 0);	 
	out_uint16(out, 0);	 

	out_uint16_le(out, format_count);
	out_uint8(out, 0);	 
	out_uint16_le(out, 2);	 
	out_uint8(out, 0);	 

	for (i = 0; i < format_count; i++)
	{
		format = &formats[i];
		out_uint16_le(out, format->wFormatTag);
		out_uint16_le(out, format->nChannels);
		out_uint32_le(out, format->nSamplesPerSec);
		out_uint32_le(out, format->nAvgBytesPerSec);
		out_uint16_le(out, format->nBlockAlign);
		out_uint16_le(out, format->wBitsPerSample);
		out_uint16(out, 0);	 
	}

	s_mark_end(out);

	logger(Sound, Debug, "rdpsnd_process_negotiate(), %d formats available",
	       (int) format_count);

	rdpsnd_send(out);

	rdpsnd_negotiated = True;
}