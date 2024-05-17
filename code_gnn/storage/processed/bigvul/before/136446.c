void ConversionContext::EndClips() {
  while (state_stack_.size() && state_stack_.back().type == StateEntry::kClip)
    EndClip();
}
