int CL_ScaledMilliseconds( void ) {
	return Sys_Milliseconds() * com_timescale->value;
}
