void ChromeClientImpl::SetToolTip(LocalFrame& frame,
                                  const String& tooltip_text,
                                  TextDirection dir) {
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(&frame)->LocalRoot();
  if (!tooltip_text.IsEmpty()) {
    web_frame->FrameWidget()->Client()->SetToolTipText(tooltip_text,
                                                       ToWebTextDirection(dir));
    did_request_non_empty_tool_tip_ = true;
  } else if (did_request_non_empty_tool_tip_) {
    web_frame->FrameWidget()->Client()->SetToolTipText(tooltip_text,
                                                       ToWebTextDirection(dir));
    did_request_non_empty_tool_tip_ = false;
  }
}
