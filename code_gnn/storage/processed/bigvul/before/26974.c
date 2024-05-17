static int do_recv_NPVariant(rpc_message_t *message, void *p_value)
{
  NPVariant *variant = (NPVariant *)p_value;
  if (variant)
	VOID_TO_NPVARIANT(*variant);

  uint32_t type;
  int error = rpc_message_recv_uint32(message, &type);
  if (error < 0)
	return error;

  NPVariant result;
  VOID_TO_NPVARIANT(result);
  switch (type) {
  case NPVariantType_Void:
	VOID_TO_NPVARIANT(result);
	break;
  case NPVariantType_Null:
	NULL_TO_NPVARIANT(result);
	break;
  case NPVariantType_Bool: {
	uint32_t value;
	if ((error = rpc_message_recv_uint32(message, &value)) < 0)
	  return error;
	result.value.boolValue = value;
	break;
  }
  case NPVariantType_Int32:
	if ((error = rpc_message_recv_int32(message, &result.value.intValue)) < 0)
	  return error;
	break;
  case NPVariantType_Double:
	if ((error = rpc_message_recv_double(message, &result.value.doubleValue)) < 0)
	  return error;
	break;
  case NPVariantType_String:
	if ((error = do_recv_NPString(message, &result.value.stringValue)) < 0)
	  return error;
	break;
  case NPVariantType_Object:
	if ((error = do_recv_NPObject(message, &result.value.objectValue)) < 0)
	  return error;
	if (NPW_IS_BROWSER) {
	   
	  NPN_RetainObject(result.value.objectValue);
	}
	break;
  }

  if (variant) {
	*variant = result;
	variant->type = type;
  }
  
  return RPC_ERROR_NO_ERROR;
}