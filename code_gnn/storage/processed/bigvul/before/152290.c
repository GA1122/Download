void RenderFrameImpl::DidRunInsecureContent(
    const blink::WebSecurityOrigin& origin,
    const blink::WebURL& target) {
  Send(new FrameHostMsg_DidRunInsecureContent(
      routing_id_, GURL(origin.ToString().Utf8()), target));
  GetContentClient()->renderer()->RecordRapporURL(
      "ContentSettings.MixedScript.RanMixedScript",
      GURL(origin.ToString().Utf8()));
}
