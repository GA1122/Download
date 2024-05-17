static gboolean lxterminal_socket_read_channel(GIOChannel * gio, GIOCondition condition, LXTermWindow * lxtermwin)
{
     
    gchar * msg = NULL;
    gsize len = 0;
    GError * err = NULL;
    GIOStatus ret = g_io_channel_read_to_end(gio, &msg, &len, &err);
    if (ret == G_IO_STATUS_ERROR)
    {
        g_warning("Error reading socket: %s\n", err->message);
    }

     
    if (len > 0)
    {
	 
        gint argc = -1;
	gsize i;
	for (i = 0; i < len; i ++)
	{
	    if (msg[i] == '\0')
	    {
	    	argc ++;
	    }
	}
	gchar * cur_dir = msg;
	gchar * * argv = g_malloc(argc * sizeof(char *));
	gint nul_count = 0;
	for (i = 0; i < len; i ++)
	{
	    if (msg[i] == '\0' && nul_count < argc)
	    {
		argv[nul_count] = &msg[i + 1];
	    	nul_count ++;
	    }
	}
         
        CommandArguments arguments;
        lxterminal_process_arguments(argc, argv, &arguments);
        g_free(argv);
	 
	if (arguments.working_directory == NULL)
	{
	    arguments.working_directory = g_strdup(cur_dir);
	}
        lxterminal_initialize(lxtermwin, &arguments);
    }
    g_free(msg);

     
    if (condition & G_IO_HUP)
    {
        return FALSE;
    }
    return TRUE;
}
