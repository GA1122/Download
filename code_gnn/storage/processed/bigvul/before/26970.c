static int do_recv_NPSetWindowCallbackStruct(rpc_message_t *message, void *p_value)
{
  NPSetWindowCallbackStruct **ws_info_p = (NPSetWindowCallbackStruct **)p_value;
  NPSetWindowCallbackStruct *ws_info;
  int32_t type;
  uint32_t has_ws_info, visual_id, colormap, depth;
  int error;

  if (ws_info_p)
	*ws_info_p = NULL;

  if ((error = rpc_message_recv_uint32(message, &has_ws_info)) < 0)
	return error;

  if (has_ws_info) {
	if ((error = rpc_message_recv_int32(message, &type)) < 0)
	  return error;
	if ((error = rpc_message_recv_uint32(message, &visual_id)) < 0)
	  return error;
	if ((error = rpc_message_recv_uint32(message, &colormap)) < 0)
	  return error;
	if ((error = rpc_message_recv_uint32(message, &depth)) < 0)
	  return error;

	if (ws_info_p) {
	  if ((ws_info = calloc(1, sizeof(*ws_info))) == NULL)
		return RPC_ERROR_NO_MEMORY;
	  ws_info->type = type;
	  ws_info->visual = (void *)(uintptr_t)visual_id;
	  ws_info->colormap = colormap;
	  ws_info->depth = depth;
	  *ws_info_p = ws_info;
	}
  }

  return RPC_ERROR_NO_ERROR;
}