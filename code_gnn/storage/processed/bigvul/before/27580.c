void midi_synth_aftertouch(int dev, int channel, int pressure)
{
	int             orig_dev = synth_devs[dev]->midi_dev;
	int             msg, chn;

	if (pressure < 0 || pressure > 127)
		return;
	if (channel < 0 || channel > 15)
		return;

	leave_sysex(dev);

	msg = prev_out_status[orig_dev] & 0xf0;
	chn = prev_out_status[orig_dev] & 0x0f;

	if (msg != 0xd0 || chn != channel)	 
	  {
		  if (!prefix_cmd(orig_dev, 0xd0 | (channel & 0x0f)))
			  return;
		  midi_outc(orig_dev, 0xd0 | (channel & 0x0f));		 
	} else if (!prefix_cmd(orig_dev, pressure))
		return;

	midi_outc(orig_dev, pressure);
}