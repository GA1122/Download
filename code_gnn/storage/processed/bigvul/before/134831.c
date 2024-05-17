  void CopyFromCompositingSurfaceCallbackForVideo(
      scoped_refptr<media::VideoFrame> video_frame,
      base::Closure quit_callback,
      bool result) {
    EXPECT_EQ(expected_copy_from_compositing_surface_result_, result);
    if (!result) {
      quit_callback.Run();
      return;
    }

    media::SkCanvasVideoRenderer video_renderer;

    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::Make(video_frame->visible_rect().width(),
                                         video_frame->visible_rect().height(),
                                         kPMColor_SkColorType,
                                         kOpaque_SkAlphaType));
    bitmap.allocPixels();
    SkCanvas canvas(bitmap);

    video_renderer.Paint(video_frame.get(),
                         &canvas,
                         video_frame->visible_rect(),
                         0xff);

    CopyFromCompositingSurfaceCallback(quit_callback,
                                       result,
                                       bitmap);
  }
