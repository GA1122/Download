void RenderViewImpl::registerIntentService(
    WebFrame* frame, const WebIntentServiceInfo& service) {
  string16 title = service.title();
  if (title.empty())
    title = webview()->mainFrame()->document().title();

  Send(new IntentsHostMsg_RegisterIntentService(routing_id_,
                                                service.action(),
                                                service.type(),
                                                service.url().spec().utf16(),
                                                title,
                                                service.disposition()));
}
