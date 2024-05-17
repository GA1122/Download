TimeRanges* HTMLMediaElement::buffered() const {
  if (media_source_)
    return media_source_->Buffered();

  if (!GetWebMediaPlayer())
    return TimeRanges::Create();

  return TimeRanges::Create(GetWebMediaPlayer()->Buffered());
}
