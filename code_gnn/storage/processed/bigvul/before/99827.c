bool WebPluginProxy::GetDragData(struct NPObject* event, bool add_data,
                                 int32* identity, int32* event_id,
                                 std::string* type, std::string* data) {
  DCHECK(event);
  NPObjectProxy* proxy = NPObjectProxy::GetProxy(event);
  if (!proxy)   
    return false;

  NPVariant_Param event_param;
  event_param.type = NPVARIANT_PARAM_RECEIVER_OBJECT_ROUTING_ID;
  event_param.npobject_routing_id = proxy->route_id();

  std::vector<NPVariant_Param> values;
  bool success = false;
  Send(new PluginHostMsg_GetDragData(route_id_, event_param, add_data,
                                     &values, &success));
  if (!success)
    return false;

  DCHECK(values.size() == 4);
  DCHECK(values[0].type == NPVARIANT_PARAM_INT);
  *identity = static_cast<int32>(values[0].int_value);
  DCHECK(values[1].type == NPVARIANT_PARAM_INT);
  *event_id = static_cast<int32>(values[1].int_value);
  DCHECK(values[2].type == NPVARIANT_PARAM_STRING);
  type->swap(values[2].string_value);
  if (add_data && (values[3].type == NPVARIANT_PARAM_STRING))
    data->swap(values[3].string_value);

  return true;
}
