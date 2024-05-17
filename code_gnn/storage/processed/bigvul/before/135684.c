static inline bool ShouldAlwaysUseDirectionalSelection(LocalFrame* frame) {
  return frame->GetEditor().Behavior().ShouldConsiderSelectionAsDirectional();
}
