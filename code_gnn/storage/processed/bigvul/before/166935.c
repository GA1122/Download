void BaseAudioContext::RecordAutoplayStatus() {
  if (!autoplay_status_.has_value())
    return;

  DEFINE_STATIC_LOCAL(
      EnumerationHistogram, autoplay_histogram,
      ("WebAudio.Autoplay", AutoplayStatus::kAutoplayStatusCount));
  DEFINE_STATIC_LOCAL(
      EnumerationHistogram, cross_origin_autoplay_histogram,
      ("WebAudio.Autoplay.CrossOrigin", AutoplayStatus::kAutoplayStatusCount));

  autoplay_histogram.Count(autoplay_status_.value());

  if (GetDocument()->GetFrame() &&
      GetDocument()->GetFrame()->IsCrossOriginSubframe()) {
    cross_origin_autoplay_histogram.Count(autoplay_status_.value());
  }

  autoplay_status_.reset();
}
