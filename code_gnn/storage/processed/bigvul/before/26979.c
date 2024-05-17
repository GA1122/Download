static int do_recv_XAnyEvent(rpc_message_t *message, XEvent *xevent)
{
  uint32_t serial, send_event, window;
  int error;
  if ((error = rpc_message_recv_uint32(message, &serial)) < 0)
	return error;
  if ((error = rpc_message_recv_uint32(message, &send_event)) < 0)
	return error;
  if ((error = rpc_message_recv_uint32(message, &window)) < 0)
	return error;
  xevent->xany.serial = serial;
  xevent->xany.send_event = send_event;
  xevent->xany.window = window;
  return RPC_ERROR_NO_ERROR;
}