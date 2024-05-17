bool WebPluginProxy::SetDropEffect(struct NPObject* event, int effect) {
  DCHECK(event);
  NPObjectProxy* proxy = NPObjectProxy::GetProxy(event);
  if (!proxy)   
    return false;

  NPVariant_Param event_param;
  event_param.type = NPVARIANT_PARAM_RECEIVER_OBJECT_ROUTING_ID;
  event_param.npobject_routing_id = proxy->route_id();

  bool success = false;
  Send(new PluginHostMsg_SetDropEffect(route_id_, event_param, effect,
                                       &success));
  return success;
}
