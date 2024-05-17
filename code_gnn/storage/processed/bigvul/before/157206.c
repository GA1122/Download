void WebMediaPlayerImpl::FinishMemoryUsageReport(int64_t demuxer_memory_usage) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  const PipelineStatistics stats = GetPipelineStatistics();
  const int64_t data_source_memory_usage =
      data_source_ ? data_source_->GetMemoryUsage() : 0;

  const int64_t video_memory_usage =
      stats.video_memory_usage +
      ((pipeline_metadata_.has_video && !stats.video_memory_usage &&
        has_first_frame_)
           ? VideoFrame::AllocationSize(PIXEL_FORMAT_I420,
                                        pipeline_metadata_.natural_size)
           : 0);

  const int64_t current_memory_usage =
      stats.audio_memory_usage + video_memory_usage + data_source_memory_usage +
      demuxer_memory_usage;

  DVLOG(2) << "Memory Usage -- Total: " << current_memory_usage
           << " Audio: " << stats.audio_memory_usage
           << ", Video: " << video_memory_usage
           << ", DataSource: " << data_source_memory_usage
           << ", Demuxer: " << demuxer_memory_usage;

  const int64_t delta = current_memory_usage - last_reported_memory_usage_;
  last_reported_memory_usage_ = current_memory_usage;
  adjust_allocated_memory_cb_.Run(delta);

  if (HasAudio()) {
    UMA_HISTOGRAM_MEMORY_KB("Media.WebMediaPlayerImpl.Memory.Audio",
                            stats.audio_memory_usage / 1024);
  }
  if (HasVideo()) {
    UMA_HISTOGRAM_MEMORY_KB("Media.WebMediaPlayerImpl.Memory.Video",
                            video_memory_usage / 1024);
  }
  if (data_source_) {
    UMA_HISTOGRAM_MEMORY_KB("Media.WebMediaPlayerImpl.Memory.DataSource",
                            data_source_memory_usage / 1024);
  }
  if (demuxer_) {
    UMA_HISTOGRAM_MEMORY_KB("Media.WebMediaPlayerImpl.Memory.Demuxer",
                            demuxer_memory_usage / 1024);
  }
}
