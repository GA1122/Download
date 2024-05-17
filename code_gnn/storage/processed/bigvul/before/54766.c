static void snd_usbmidi_emagic_finish_out(struct snd_usb_midi_out_endpoint *ep)
{
	static const u8 finish_data[] = {
		 
		0xf0,
		0x00, 0x20, 0x31,	 
		0x64,			 
		0x10,			 
		0x00,			 
		0x7f,			 
		0x40,			 
		0xf7
	};
	send_bulk_static_data(ep, finish_data, sizeof(finish_data));
}
