skia::RefPtr<SkPicture> BrowserViewRenderer::CapturePicture(int width,
                                                            int height) {
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::CapturePicture");

  if (width <= 0 || height <= 0) {
    SkPictureRecorder emptyRecorder;
    emptyRecorder.beginRecording(0, 0);
    return skia::AdoptRef(emptyRecorder.endRecording());
  }

  SkPictureRecorder recorder;
  SkCanvas* rec_canvas = recorder.beginRecording(width, height, NULL, 0);
  if (compositor_) {
    {
      compositor_->DidChangeRootLayerScrollOffset(gfx::ScrollOffset());
      CompositeSW(rec_canvas);
    }
    compositor_->DidChangeRootLayerScrollOffset(
        gfx::ScrollOffset(scroll_offset_dip_));
  }
  return skia::AdoptRef(recorder.endRecording());
}
