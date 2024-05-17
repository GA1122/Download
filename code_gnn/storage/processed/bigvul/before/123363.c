void RenderWidgetHostViewGuest::Destroy() {
  host_ = NULL;
  MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
