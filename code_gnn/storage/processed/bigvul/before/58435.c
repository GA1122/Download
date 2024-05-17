close_socket (int sok)
{
	 
	fe_timeout_add (5000, close_socket_cb, GINT_TO_POINTER (sok));
}
