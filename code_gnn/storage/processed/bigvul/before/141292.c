String Document::UserAgent() const {
  return GetFrame() ? GetFrame()->Loader().UserAgent() : String();
}
