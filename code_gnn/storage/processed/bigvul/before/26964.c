static int do_recv_NPObject(rpc_message_t *message, void *p_value)
{
  int error;
  uint32_t npobj_id;

  if ((error = rpc_message_recv_uint32(message, &npobj_id)) < 0)
	return error;

  NPObject *npobj = NULL;
  if (npobj_id) {
	npobj = npobject_lookup(npobj_id);
#ifdef BUILD_VIEWER
	if (npobj == NULL) {
	  if ((npobj = npobject_new(npobj_id, NULL, NULL)) == NULL)
		return RPC_ERROR_NO_MEMORY;
	}
#endif
	assert(npobj != NULL);

#ifdef BUILD_VIEWER
	uint32_t referenceCount;
	if ((error = rpc_message_recv_uint32(message, &referenceCount)) < 0)
	  return error;
	if (npobj->referenceCount != referenceCount) {
	  D(bug("synchronize NPObject::referenceCount (%d -> %d)\n",
			npobj->referenceCount, referenceCount));
	  npobj->referenceCount = referenceCount;
	}
#endif
  }

  *((NPObject **)p_value) = npobj;
  return RPC_ERROR_NO_ERROR;
}