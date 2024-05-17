void WebGLRenderingContextBase::PushFrame() {
  if (!marked_canvas_dirty_)
    return;

  marked_canvas_dirty_ = false;
  int width = GetDrawingBuffer()->Size().Width();
  int height = GetDrawingBuffer()->Size().Height();
  if (!GetDrawingBuffer()) {
    return Host()->PushFrame(nullptr, SkIRect::MakeWH(width, height));
  }

  std::unique_ptr<viz::SingleReleaseCallback> image_release_callback;
  scoped_refptr<StaticBitmapImage> image =
      GetStaticBitmapImage(&image_release_callback);
  GetDrawingBuffer()->SwapPreviousFrameCallback(
      std::move(image_release_callback));
  return Host()->PushFrame(std::move(image), SkIRect::MakeWH(width, height));
}
