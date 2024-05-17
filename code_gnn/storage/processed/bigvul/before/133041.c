void RenderWidgetHostViewAura::SwapBuffersCompleted(
    const BufferPresentedCallback& ack_callback,
    const scoped_refptr<ui::Texture>& texture_to_return) {
  ui::Compositor* compositor = GetCompositor();
  if (!compositor) {
    ack_callback.Run(false, texture_to_return);
  } else {
    AddOnCommitCallbackAndDisableLocks(
        base::Bind(ack_callback, false, texture_to_return));
  }

  DidReceiveFrameFromRenderer();
}
