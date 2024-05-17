void DelegatedFrameHost::CopyFromCompositingSurfaceHasResultForVideo(
    base::WeakPtr<DelegatedFrameHost> dfh,
    scoped_refptr<OwnedMailbox> subscriber_texture,
    scoped_refptr<media::VideoFrame> video_frame,
    const base::Callback<void(const gfx::Rect&, bool)>& callback,
    std::unique_ptr<viz::CopyOutputResult> result) {
  base::ScopedClosureRunner scoped_callback_runner(
      base::Bind(callback, gfx::Rect(), false));
  base::ScopedClosureRunner scoped_return_subscriber_texture(base::Bind(
      &ReturnSubscriberTexture, dfh, subscriber_texture, gpu::SyncToken()));

  if (!dfh)
    return;
  if (result->IsEmpty())
    return;
  if (result->size().IsEmpty())
    return;

  gfx::Rect region_in_frame = media::ComputeLetterboxRegion(
      video_frame->visible_rect(), result->size());
  region_in_frame =
      gfx::Rect(region_in_frame.x() & ~1, region_in_frame.y() & ~1,
                region_in_frame.width() & ~1, region_in_frame.height() & ~1);
  if (region_in_frame.IsEmpty())
    return;

  if (!result->HasTexture()) {
    DCHECK(result->HasBitmap());
    std::unique_ptr<SkBitmap> bitmap = result->TakeBitmap();
    SkBitmap scaled_bitmap;
    if (result->size() != region_in_frame.size()) {
      skia::ImageOperations::ResizeMethod method =
          skia::ImageOperations::RESIZE_GOOD;
      scaled_bitmap = skia::ImageOperations::Resize(*bitmap.get(), method,
                                                    region_in_frame.width(),
                                                    region_in_frame.height());
    } else {
      scaled_bitmap = *bitmap.get();
    }

    media::CopyRGBToVideoFrame(
        reinterpret_cast<uint8_t*>(scaled_bitmap.getPixels()),
        scaled_bitmap.rowBytes(), region_in_frame, video_frame.get());

    ignore_result(scoped_callback_runner.Release());
    callback.Run(region_in_frame, true);
    return;
  }

  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  viz::GLHelper* gl_helper = factory->GetGLHelper();
  if (!gl_helper)
    return;
  if (subscriber_texture.get() && !subscriber_texture->texture_id())
    return;

  viz::TextureMailbox texture_mailbox;
  std::unique_ptr<viz::SingleReleaseCallback> release_callback;
  result->TakeTexture(&texture_mailbox, &release_callback);
  DCHECK(texture_mailbox.IsTexture());

  gfx::Rect result_rect(result->size());

  viz::ReadbackYUVInterface* yuv_readback_pipeline =
      dfh->yuv_readback_pipeline_.get();
  if (yuv_readback_pipeline == NULL ||
      yuv_readback_pipeline->scaler()->SrcSize() != result_rect.size() ||
      yuv_readback_pipeline->scaler()->SrcSubrect() != result_rect ||
      yuv_readback_pipeline->scaler()->DstSize() != region_in_frame.size()) {
    viz::GLHelper::ScalerQuality quality =
        ((result_rect.size().width() < region_in_frame.size().width()) &&
         (result_rect.size().height() < region_in_frame.size().height()))
            ? viz::GLHelper::SCALER_QUALITY_BEST
            : viz::GLHelper::SCALER_QUALITY_FAST;

    DVLOG(1) << "Re-creating YUV readback pipeline for source rect "
             << result_rect.ToString() << " and destination size "
             << region_in_frame.size().ToString();

    dfh->yuv_readback_pipeline_.reset(gl_helper->CreateReadbackPipelineYUV(
        quality, result_rect.size(), result_rect, region_in_frame.size(), true,
        true));
    yuv_readback_pipeline = dfh->yuv_readback_pipeline_.get();
  }

  ignore_result(scoped_callback_runner.Release());
  ignore_result(scoped_return_subscriber_texture.Release());

  base::Callback<void(bool result)> finished_callback = base::Bind(
      &DelegatedFrameHost::CopyFromCompositingSurfaceFinishedForVideo,
      video_frame, dfh->AsWeakPtr(), base::Bind(callback, region_in_frame),
      subscriber_texture, base::Passed(&release_callback));
  yuv_readback_pipeline->ReadbackYUV(
      texture_mailbox.mailbox(), texture_mailbox.sync_token(),
      video_frame->visible_rect(),
      video_frame->stride(media::VideoFrame::kYPlane),
      video_frame->data(media::VideoFrame::kYPlane),
      video_frame->stride(media::VideoFrame::kUPlane),
      video_frame->data(media::VideoFrame::kUPlane),
      video_frame->stride(media::VideoFrame::kVPlane),
      video_frame->data(media::VideoFrame::kVPlane), region_in_frame.origin(),
      finished_callback);
  media::LetterboxYUV(video_frame.get(), region_in_frame);
}
