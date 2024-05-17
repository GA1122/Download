void RenderViewImpl::registerIntentService(
    WebFrame* frame, const WebIntentServiceInfo& service) {
  webkit_glue::WebIntentServiceData data(service);
  if (data.title.empty())
    data.title = webview()->mainFrame()->document().title();
  bool user_gesture = frame->isProcessingUserGesture();
  Send(new IntentsHostMsg_RegisterIntentService(routing_id_,
                                                data,
                                                user_gesture));
}
