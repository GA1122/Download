static void snd_usbmidi_emagic_init_out(struct snd_usb_midi_out_endpoint *ep)
{
	static const u8 init_data[] = {
		 
		0xf0,
		0x00, 0x20, 0x31,	 
		0x64,			 
		0x0b,			 
		0x00,			 
		0x00,			 
		0xf7
	};
	send_bulk_static_data(ep, init_data, sizeof(init_data));
	 
	send_bulk_static_data(ep, init_data, sizeof(init_data));
}
