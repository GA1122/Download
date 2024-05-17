void RenderViewImpl::OnJavaBridgeInit() {
  DCHECK(!java_bridge_dispatcher_.get());
#if defined(ENABLE_JAVA_BRIDGE)
  java_bridge_dispatcher_.reset(new JavaBridgeDispatcher(this));
#endif
}
