  void PerformTestWithLeftRightRects(const gfx::Size& html_rect_size,
                                     const gfx::Rect& copy_rect,
                                     const gfx::Size& output_size,
                                     const gfx::Size& expected_bitmap_size,
                                     bool video_frame) {
    const gfx::Size box_size(html_rect_size.width() / 2,
                             html_rect_size.height());
    SetTestUrl(base::StringPrintf(
        "data:text/html,<!doctype html>"
        "<div class='left'>"
        "  <div class='right'></div>"
        "</div>"
        "<style>"
        "body { padding: 0; margin: 0; }"
        ".left { position: absolute;"
        "        background: #0ff;"
        "        width: %dpx;"
        "        height: %dpx;"
        "}"
        ".right { position: absolute;"
        "         left: %dpx;"
        "         background: #ff0;"
        "         width: %dpx;"
        "         height: %dpx;"
        "}"
        "</style>"
        "<script>"
        "  domAutomationController.setAutomationId(0);"
        "  domAutomationController.send(\"DONE\");"
        "</script>",
        box_size.width(),
        box_size.height(),
        box_size.width(),
        box_size.width(),
        box_size.height()));

    SET_UP_SURFACE_OR_PASS_TEST("\"DONE\"");
    if (!ShouldContinueAfterTestURLLoad())
      return;

    RenderWidgetHostViewPort* rwhvp = GetRenderWidgetHostViewPort();
    if (video_frame && !rwhvp->CanCopyToVideoFrame()) {
#if defined(OS_MACOSX)
      if (!content::GpuDataManager::GetInstance()->GpuAccessAllowed(NULL)) {
        LOG(WARNING) << ("Blindly passing this test because copying to "
                         "video frames is not supported on this platform.");
        return;
      }
#endif
      NOTREACHED();
    }

    uint32 frame = rwhvp->RendererFrameNumber();
    while (!GetRenderWidgetHost()->ScheduleComposite())
      GiveItSomeTime();
    while (rwhvp->RendererFrameNumber() == frame)
      GiveItSomeTime();

    SkBitmap expected_bitmap;
    SetupLeftRightBitmap(expected_bitmap_size, &expected_bitmap);
    SetExpectedCopyFromCompositingSurfaceResult(true, expected_bitmap);

    base::RunLoop run_loop;
    if (video_frame) {
      SetAllowableError(0x10);
      SetExcludeRect(
          gfx::Rect(output_size.width() / 2 - 1, 0, 2, output_size.height()));

      scoped_refptr<media::VideoFrame> video_frame =
          media::VideoFrame::CreateFrame(media::VideoFrame::YV12,
                                         expected_bitmap_size,
                                         gfx::Rect(expected_bitmap_size),
                                         expected_bitmap_size,
                                         base::TimeDelta());

      base::Callback<void(bool success)> callback =
          base::Bind(&CompositingRenderWidgetHostViewBrowserTestTabCapture::
                         CopyFromCompositingSurfaceCallbackForVideo,
                     base::Unretained(this),
                     video_frame,
                     run_loop.QuitClosure());
      rwhvp->CopyFromCompositingSurfaceToVideoFrame(copy_rect,
                                                    video_frame,
                                                    callback);
    } else {
#if defined(USE_AURA)
      if (!content::GpuDataManager::GetInstance()
               ->CanUseGpuBrowserCompositor()) {
        SetAllowableError(2);
        SetExcludeRect(
            gfx::Rect(output_size.width() / 2 - 1, 0, 2, output_size.height()));
      }
#endif

      base::Callback<void(bool, const SkBitmap&)> callback =
          base::Bind(&CompositingRenderWidgetHostViewBrowserTestTabCapture::
                       CopyFromCompositingSurfaceCallback,
                   base::Unretained(this),
                   run_loop.QuitClosure());
      rwhvp->CopyFromCompositingSurface(copy_rect,
                                        output_size,
                                        callback,
                                        SkBitmap::kARGB_8888_Config);
    }
    run_loop.Run();
  }
