void PageHandler::ScreencastFrameCaptured(viz::CompositorFrameMetadata metadata,
                                          const SkBitmap& bitmap,
                                          ReadbackResponse response) {
  if (response != READBACK_SUCCESS) {
    if (capture_retry_count_) {
      --capture_retry_count_;
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE,
          base::BindOnce(&PageHandler::InnerSwapCompositorFrame,
                         weak_factory_.GetWeakPtr()),
          base::TimeDelta::FromMilliseconds(kFrameRetryDelayMs));
    }
    --frames_in_flight_;
    return;
  }
  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::Bind(&EncodeSkBitmap, bitmap, screencast_format_,
                 screencast_quality_),
      base::Bind(&PageHandler::ScreencastFrameEncoded,
                 weak_factory_.GetWeakPtr(), base::Passed(&metadata),
                 base::Time::Now()));
}
