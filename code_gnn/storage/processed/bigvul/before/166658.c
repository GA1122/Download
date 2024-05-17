void WebGLRenderingContextBase::commit() {
  int width = GetDrawingBuffer()->Size().Width();
  int height = GetDrawingBuffer()->Size().Height();

  std::unique_ptr<viz::SingleReleaseCallback> image_release_callback;
  scoped_refptr<StaticBitmapImage> image =
      GetStaticBitmapImage(&image_release_callback);
  GetDrawingBuffer()->SwapPreviousFrameCallback(
      std::move(image_release_callback));

  Host()->Commit(std::move(image), SkIRect::MakeWH(width, height));
}
