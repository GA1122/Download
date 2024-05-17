const AtomicString& TextTrack::MetadataKeyword() {
  DEFINE_STATIC_LOCAL(const AtomicString, metadata, ("metadata"));
  return metadata;
}
