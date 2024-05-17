bool WebMediaPlayerImpl::IsBackgroundOptimizationCandidate() const {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (IsInPictureInPicture())
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

  if (duration < kMaxKeyframeDistance)
    return true;

  PipelineStatistics stats = GetPipelineStatistics();
  return stats.video_keyframe_distance_average < kMaxKeyframeDistance;
}
