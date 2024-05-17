static int handle_NP_Shutdown(rpc_connection_t *connection)
{
  D(bug("handle_NP_Shutdown\n"));

  int error = rpc_method_get_args(connection, RPC_TYPE_INVALID);
  if (error != RPC_ERROR_NO_ERROR) {
	npw_perror("NP_Shutdown() get args", error);
	return error;
  }

   
  delayed_destroys_process_cb(NULL);

  NPError ret = g_NP_Shutdown();
  return rpc_method_send_reply(connection, RPC_TYPE_INT32, ret, RPC_TYPE_INVALID);
}