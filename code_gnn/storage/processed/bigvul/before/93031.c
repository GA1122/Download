rdp_reset_state(void)
{
	logger(Protocol, Debug, "%s()", __func__);
	g_next_packet = NULL;	 
	g_rdp_shareid = 0;
	g_exit_mainloop = False;
	g_first_bitmap_caps = True;
	sec_reset_state();
}
