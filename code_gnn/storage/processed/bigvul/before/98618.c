void ThumbnailGenerator::ShownDelayHandler() {
  base::TimeTicks threshold = base::TimeTicks::Now() -
      base::TimeDelta::FromMilliseconds(kVisibilitySlopMS);

  for (size_t i = 0; i < shown_hosts_.size(); i++) {
    WidgetThumbnail* wt = GetDataForHost(shown_hosts_[i]);
    if (no_timeout_ || wt->last_shown <= threshold) {
      wt->thumbnail = SkBitmap();
      shown_hosts_.erase(shown_hosts_.begin() + i);
      i--;
    }
  }

  if (!shown_hosts_.empty()) {
    DCHECK(!no_timeout_);
    timer_.Start(base::TimeDelta::FromMilliseconds(kVisibilitySlopMS) / 2, this,
                 &ThumbnailGenerator::ShownDelayHandler);
  }
}
