bool PasswordAutofillAgent::FrameCanAccessPasswordManager() {
  WebLocalFrame* frame = render_frame()->GetWebFrame();
  blink::WebURL url = frame->GetDocument().Url();
  if (!url.ProtocolIs(url::kHttpScheme) && !url.ProtocolIs(url::kHttpsScheme))
    return false;
  return frame->GetSecurityOrigin().CanAccessPasswordManager();
}
