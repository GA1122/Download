void ConversionContext::PushState(StateEntry::PairedType type,
                                  int saved_count) {
  state_stack_.emplace_back(StateEntry{type, saved_count, current_transform_,
                                       current_clip_, current_effect_,
                                       previous_transform_});
  previous_transform_ = nullptr;
}
