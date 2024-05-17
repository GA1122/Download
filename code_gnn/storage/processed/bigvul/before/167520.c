bool WebMediaPlayerImpl::IsBackgroundOptimizationCandidate() const {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (client_->DisplayType() == WebMediaPlayer::DisplayType::kPictureInPicture)
    return false;

#if defined(OS_ANDROID)   
  if (IsRemote())
    return false;
#endif   

  if (!HasVideo() || IsStreaming())
    return false;

  if (!HasAudio() && HasVideo())
    return true;

  base::TimeDelta duration = GetPipelineMediaDuration();
  base::TimeDelta max_keyframe_distance =
      (load_type_ == kLoadTypeMediaSource)
          ? max_keyframe_distance_to_disable_background_video_mse_
          : max_keyframe_distance_to_disable_background_video_;
  if (duration < max_keyframe_distance)
    return true;

  PipelineStatistics stats = GetPipelineStatistics();
  return stats.video_keyframe_distance_average < max_keyframe_distance;
}
