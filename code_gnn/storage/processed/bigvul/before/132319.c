void RenderFrameImpl::didReceiveTitle(blink::WebLocalFrame* frame,
                                      const blink::WebString& title,
                                      blink::WebTextDirection direction) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->parent()) {
    base::string16 title16 = title;
    base::trace_event::TraceLog::GetInstance()->UpdateProcessLabel(
        routing_id_, base::UTF16ToUTF8(title16));

    base::string16 shortened_title = title16.substr(0, kMaxTitleChars);
    Send(new FrameHostMsg_UpdateTitle(routing_id_,
                                      shortened_title, direction));
  }

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
