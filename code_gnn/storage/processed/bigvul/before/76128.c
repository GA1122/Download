notify_fifo_open(notify_fifo_t* global_fifo, notify_fifo_t* fifo, int (*script_exit)(thread_t *), const char *type)
{
	 
	if (global_fifo->name)
		fifo_open(global_fifo, script_exit, "");

	 
	if (fifo->name)
		fifo_open(fifo, script_exit, type);
}
