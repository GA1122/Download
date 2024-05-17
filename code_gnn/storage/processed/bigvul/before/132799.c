skia::RefPtr<SkPicture> PictureLayer::GetPicture() const {
  if (!DrawsContent())
    return skia::RefPtr<SkPicture>();

  int width = bounds().width();
  int height = bounds().height();

  SkPictureRecorder recorder;
  SkCanvas* canvas = recorder.beginRecording(width, height, nullptr, 0);
  client_->PaintContents(canvas,
                         gfx::Rect(width, height),
                         ContentLayerClient::GRAPHICS_CONTEXT_ENABLED);
  skia::RefPtr<SkPicture> picture = skia::AdoptRef(recorder.endRecording());
  return picture;
}
