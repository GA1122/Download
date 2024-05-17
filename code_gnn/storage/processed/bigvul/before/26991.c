static int do_send_NPObject(rpc_message_t *message, void *p_value)
{
  uint32_t npobj_id = 0;
  NPObject *npobj = (NPObject *)p_value;
  if (npobj) {
	NPObjectInfo *npobj_info = npobject_info_lookup(npobj);
	if (npobj_info)
	  npobj_id = npobj_info->npobj_id;
#ifdef BUILD_WRAPPER
	else {
	  if ((npobj_info = npobject_info_new(npobj)) == NULL)
		return RPC_ERROR_NO_MEMORY;
	  npobj_id = npobj_info->npobj_id;
	  npobject_associate(npobj, npobj_info);
	}
#endif
	assert(npobj_id != 0);
  }
  int error = rpc_message_send_uint32(message, npobj_id);
  if (error < 0)
	return error;

#ifdef BUILD_WRAPPER
  if (npobj) {
	if ((error = rpc_message_send_uint32(message, npobj->referenceCount)) < 0)
	  return error;
  }
#endif

  return RPC_ERROR_NO_ERROR;
}