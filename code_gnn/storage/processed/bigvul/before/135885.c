const AtomicString& TextTrack::SubtitlesKeyword() {
  DEFINE_STATIC_LOCAL(const AtomicString, subtitles, ("subtitles"));
  return subtitles;
}
