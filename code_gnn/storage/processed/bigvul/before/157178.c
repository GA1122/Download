blink::WebTimeRanges WebMediaPlayerImpl::Buffered() const {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  Ranges<base::TimeDelta> buffered_time_ranges =
      pipeline_controller_.GetBufferedTimeRanges();

  const base::TimeDelta duration = GetPipelineMediaDuration();
  if (duration != kInfiniteDuration) {
    buffered_data_source_host_.AddBufferedTimeRanges(&buffered_time_ranges,
                                                     duration);
  }
  return ConvertToWebTimeRanges(buffered_time_ranges);
}
