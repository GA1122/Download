void WebPluginDelegateProxy::OnGetDragData(const NPVariant_Param& object,
                                           bool add_data,
                                           std::vector<NPVariant_Param>* values,
                                           bool* success) {
  DCHECK(values && success);
  *success = false;

  WebView* webview = NULL;
  if (render_view_)
    webview = render_view_->webview();
  if (!webview)
    return;

  int event_id;
  WebDragData data;

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
  if (!drag_id || !WebBindings::getDragData(event, &event_id, &data))
    return;

  NPVariant results[4];
  INT32_TO_NPVARIANT(drag_id, results[0]);
  INT32_TO_NPVARIANT(event_id, results[1]);
  EncodeDragData(data, add_data, &results[2], &results[3]);

  for (size_t i = 0; i < arraysize(results); ++i) {
    values->push_back(NPVariant_Param());
    CreateNPVariantParam(
        results[i], NULL, &values->back(), false, 0, page_url_);
  }

  *success = true;
}
