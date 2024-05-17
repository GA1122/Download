static int i8042_set_mux_mode(bool multiplex, unsigned char *mux_version)
{

	unsigned char param, val;
 

	i8042_flush();

 

	param = val = 0xf0;
	if (i8042_command(&param, I8042_CMD_AUX_LOOP) || param != val)
		return -1;
	param = val = multiplex ? 0x56 : 0xf6;
	if (i8042_command(&param, I8042_CMD_AUX_LOOP) || param != val)
		return -1;
	param = val = multiplex ? 0xa4 : 0xa5;
	if (i8042_command(&param, I8042_CMD_AUX_LOOP) || param == val)
		return -1;

 
	if (param == 0xac)
		return -1;

	if (mux_version)
		*mux_version = param;

	return 0;
}
