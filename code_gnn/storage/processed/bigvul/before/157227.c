bool IsBackgroundVideoTrackOptimizationEnabled(
    WebMediaPlayer::LoadType load_type) {
  return load_type == WebMediaPlayer::LoadType::kLoadTypeMediaSource ||
         base::FeatureList::IsEnabled(kBackgroundSrcVideoTrackOptimization);
}
