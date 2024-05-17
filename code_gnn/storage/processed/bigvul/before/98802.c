void WebPluginDelegateProxy::OnSetDropEffect(const NPVariant_Param& object,
                                             int effect,
                                             bool* success) {
  DCHECK(success);
  *success = false;

  WebView* webview = NULL;
  if (render_view_)
    webview = render_view_->webview();
  if (!webview)
    return;

  DCHECK(object.type == NPVARIANT_PARAM_RECEIVER_OBJECT_ROUTING_ID);
  NPObjectBase* npobject_base =
      channel_host_->GetNPObjectListenerForRoute(object.npobject_routing_id);
  if (!npobject_base) {
    DLOG(WARNING) << "Invalid routing id passed in"
                  << object.npobject_routing_id;
    return;
  }

  NPObject* event = npobject_base->GetUnderlyingNPObject();
  DCHECK(event != NULL);

  const int32 drag_id = webview->dragIdentity();
  if (!drag_id || !WebBindings::isDragEvent(event))
    return;

  *success = webview->setDropEffect(effect != 0);
}
