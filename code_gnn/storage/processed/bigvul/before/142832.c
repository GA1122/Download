void HTMLMediaElement::LoadSourceFromObject() {
  DCHECK(src_object_);
  load_state_ = kLoadingFromSrcObject;

  LoadResource(WebMediaPlayerSource(WebMediaStream(src_object_)), String());
}
