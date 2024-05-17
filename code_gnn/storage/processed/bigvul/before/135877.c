bool TextTrack::IsValidKindKeyword(const String& value) {
  if (value == SubtitlesKeyword())
    return true;
  if (value == CaptionsKeyword())
    return true;
  if (value == DescriptionsKeyword())
    return true;
  if (value == ChaptersKeyword())
    return true;
  if (value == MetadataKeyword())
    return true;

  return false;
}
