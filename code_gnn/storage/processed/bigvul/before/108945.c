void RenderViewImpl::OnJavaBridgeInit() {
  DCHECK(!java_bridge_dispatcher_);
#if defined(ENABLE_JAVA_BRIDGE)
  java_bridge_dispatcher_ = new JavaBridgeDispatcher(this);
#endif
}
