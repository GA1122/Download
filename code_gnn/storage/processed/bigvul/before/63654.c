static int irssi_io_invoke(GIOChannel *source, GIOCondition condition,
			   void *data)
{
	IRSSI_INPUT_REC *rec = data;
	int icond = 0;

	if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL)) {
		 
		if (rec->condition & G_IO_IN)
			icond |= G_INPUT_READ;
		else
			icond |= G_INPUT_WRITE;
	}

	if (condition & (G_IO_IN | G_IO_PRI))
		icond |= G_INPUT_READ;
	if (condition & G_IO_OUT)
		icond |= G_INPUT_WRITE;

	if (rec->condition & icond)
		rec->function(rec->data, source, icond);

	return TRUE;
}