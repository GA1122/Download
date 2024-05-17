void ConversionContext::EndClip() {
  DCHECK_EQ(state_stack_.back().type, StateEntry::kClip);
  DCHECK_EQ(state_stack_.back().effect, current_effect_);
  EndTransform();
  PopState();
}
