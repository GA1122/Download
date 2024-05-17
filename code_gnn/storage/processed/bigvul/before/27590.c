midi_synth_send_sysex(int dev, unsigned char *bytes, int len)
{
	int             orig_dev = synth_devs[dev]->midi_dev;
	int             i;

	for (i = 0; i < len; i++)
	  {
		  switch (bytes[i])
		    {
		    case 0xf0:	 
			    if (!prefix_cmd(orig_dev, 0xf0))
				    return 0;
			    sysex_state[dev] = 1;
			    break;

		    case 0xf7:	 
			    if (!sysex_state[dev])	 
				    return 0;
			    sysex_state[dev] = 0;
			    break;

		    default:
			    if (!sysex_state[dev])
				    return 0;

			    if (bytes[i] & 0x80)	 
			      {
				      bytes[i] = 0xf7;	 
				      sysex_state[dev] = 0;
			      }
		    }

		  if (!midi_devs[orig_dev]->outputc(orig_dev, bytes[i]))
		    {
 

			    int             timeout = 0;

			    bytes[i] = 0xf7;
			    sysex_state[dev] = 0;

			    while (!midi_devs[orig_dev]->outputc(orig_dev, bytes[i]) &&
				   timeout < 1000)
				    timeout++;
		    }
		  if (!sysex_state[dev])
			  return 0;
	  }

	return 0;
}
