void ChromeClientImpl::RequestUnbufferedInputEvents(LocalFrame* frame) {
  DCHECK(frame);
  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRoot()->FrameWidget();
  if (!widget)
    return;

  if (WebWidgetClient* client = widget->Client())
    client->RequestUnbufferedInputEvents();
}
