void WebLocalFrameImpl::DispatchMessageEventWithOriginCheck(
    const WebSecurityOrigin& intended_target_origin,
    const WebDOMEvent& event) {
  DCHECK(!event.IsNull());
  GetFrame()->DomWindow()->DispatchMessageEventWithOriginCheck(
      intended_target_origin.Get(), event,
      SourceLocation::Create(String(), 0, 0, nullptr));
}
