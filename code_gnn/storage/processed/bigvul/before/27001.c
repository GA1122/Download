static int do_send_NPVariant(rpc_message_t *message, void *p_value)
{
  NPVariant *variant = (NPVariant *)p_value;
  if (variant == NULL)
	return RPC_ERROR_MESSAGE_ARGUMENT_INVALID;

  int error = rpc_message_send_uint32(message, variant->type);
  if (error < 0)
	return error;

  switch (variant->type) {
  case NPVariantType_Void:
  case NPVariantType_Null:
	break;
  case NPVariantType_Bool:
	if ((error = rpc_message_send_uint32(message, variant->value.boolValue)) < 0)
	  return error;
	break;
  case NPVariantType_Int32:
	if ((error = rpc_message_send_int32(message, variant->value.intValue)) < 0)
	  return error;
	break;
  case NPVariantType_Double:
	if ((error = rpc_message_send_double(message, variant->value.doubleValue)) < 0)
	  return error;
	break;
  case NPVariantType_String:
	if ((error = do_send_NPString(message, &variant->value.stringValue)) < 0)
	  return error;
	break;
  case NPVariantType_Object:
	if (NPW_IS_BROWSER) {
	   
	  NPN_RetainObject(variant->value.objectValue);
	}
	if ((error = do_send_NPObject(message, variant->value.objectValue)) < 0)
	  return error;
	break;
  }

  return RPC_ERROR_NO_ERROR;
}