static int do_recv_NPFullPrint(rpc_message_t *message, void *p_value)
{
  NPFullPrint *fullPrint = (NPFullPrint *)p_value;
  uint32_t pluginPrinted, printOne;
  int error;

  if ((error = rpc_message_recv_uint32(message, &pluginPrinted)) < 0)
	return error;
  if ((error = rpc_message_recv_uint32(message, &printOne)) < 0)
	return error;

  fullPrint->pluginPrinted = pluginPrinted;
  fullPrint->printOne = printOne;
  fullPrint->platformPrint = NULL;  
  return RPC_ERROR_NO_ERROR;
}