String FrameSelection::SelectedText(
    const TextIteratorBehavior& behavior) const {
  return ExtractSelectedText(*this, behavior);
}
