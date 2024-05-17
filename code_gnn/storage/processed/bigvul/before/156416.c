void PageHandler::ScreencastFrameCaptured(
    std::unique_ptr<Page::ScreencastFrameMetadata> page_metadata,
    const SkBitmap& bitmap) {
  if (bitmap.drawsNothing()) {
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
      base::BindOnce(&EncodeSkBitmap, bitmap, screencast_format_,
                     screencast_quality_),
      base::BindOnce(&PageHandler::ScreencastFrameEncoded,
                     weak_factory_.GetWeakPtr(), std::move(page_metadata)));
}
