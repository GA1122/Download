blink::WebRTCSessionDescription CreateWebKitSessionDescription(
    const std::string& sdp, const std::string& type) {
  blink::WebRTCSessionDescription description;
  description.Initialize(blink::WebString::FromUTF8(type),
                         blink::WebString::FromUTF8(sdp));
  return description;
}
