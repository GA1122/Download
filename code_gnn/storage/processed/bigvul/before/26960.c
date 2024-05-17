static int do_recv_NPEmbedPrint(rpc_message_t *message, void *p_value)
{
  NPEmbedPrint *embedPrint = (NPEmbedPrint *)p_value;
  int error;

  if ((error = do_recv_NPWindowData(message, &embedPrint->window)) < 0)
	return error;

  embedPrint->platformPrint = NULL;  
  return RPC_ERROR_NO_ERROR;
}
