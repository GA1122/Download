TimeRanges* HTMLMediaElement::seekable() const {
  if (!GetWebMediaPlayer())
    return TimeRanges::Create();

  if (media_source_)
    return media_source_->Seekable();

  return TimeRanges::Create(GetWebMediaPlayer()->Seekable());
}
