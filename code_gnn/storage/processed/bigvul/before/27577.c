do_midi_msg(int synthno, unsigned char *msg, int mlen)
{
	switch (msg[0] & 0xf0)
	  {
	  case 0x90:
		  if (msg[2] != 0)
		    {
			    STORE(SEQ_START_NOTE(synthno, msg[0] & 0x0f, msg[1], msg[2]));
			    break;
		    }
		  msg[2] = 64;

	  case 0x80:
		  STORE(SEQ_STOP_NOTE(synthno, msg[0] & 0x0f, msg[1], msg[2]));
		  break;

	  case 0xA0:
		  STORE(SEQ_KEY_PRESSURE(synthno, msg[0] & 0x0f, msg[1], msg[2]));
		  break;

	  case 0xB0:
		  STORE(SEQ_CONTROL(synthno, msg[0] & 0x0f,
				    msg[1], msg[2]));
		  break;

	  case 0xC0:
		  STORE(SEQ_SET_PATCH(synthno, msg[0] & 0x0f, msg[1]));
		  break;

	  case 0xD0:
		  STORE(SEQ_CHN_PRESSURE(synthno, msg[0] & 0x0f, msg[1]));
		  break;

	  case 0xE0:
		  STORE(SEQ_BENDER(synthno, msg[0] & 0x0f,
			      (msg[1] & 0x7f) | ((msg[2] & 0x7f) << 7)));
		  break;

	  default:
		   
		  ;
	  }
}