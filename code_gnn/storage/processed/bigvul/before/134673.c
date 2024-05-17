skia::RefPtr<SkPicture> BrowserViewRenderer::CapturePicture(int width,
                                                            int height) {
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::CapturePicture");

  if (width <= 0 || height <= 0) {
    SkPictureRecorder emptyRecorder;
    emptyRecorder.beginRecording(0, 0);
    return skia::AdoptRef(emptyRecorder.endRecording());
  }

  base::AutoReset<gfx::Vector2dF> scroll_reset(&scroll_offset_dip_,
                                               gfx::Vector2dF());

  SkPictureRecorder recorder;
  SkCanvas* rec_canvas = recorder.beginRecording(width, height, NULL, 0);
  if (compositor_)
    CompositeSW(rec_canvas);
  return skia::AdoptRef(recorder.endRecording());
}
