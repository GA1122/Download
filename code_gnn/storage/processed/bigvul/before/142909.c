void HTMLMediaElement::SetSrcObject(MediaStreamDescriptor* src_object) {
  BLINK_MEDIA_LOG << "setSrcObject(" << (void*)this << ")";
  src_object_ = src_object;
  InvokeLoadAlgorithm();
}
