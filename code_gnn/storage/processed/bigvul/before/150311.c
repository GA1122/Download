bool IsInCrossOriginIframe(const WebInputElement& element) {
  WebFrame* cur_frame = element.GetDocument().GetFrame();
  WebString bottom_frame_origin = cur_frame->GetSecurityOrigin().ToString();

  DCHECK(cur_frame);

  while (cur_frame->Parent()) {
    cur_frame = cur_frame->Parent();
    if (!IsPublicSuffixDomainMatch(
            bottom_frame_origin.Utf8(),
            cur_frame->GetSecurityOrigin().ToString().Utf8())) {
      return true;
    }
  }
  return false;
}
