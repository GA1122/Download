DEFINE_TRACE(FrameSelection) {
  visitor->Trace(frame_);
  visitor->Trace(layout_selection_);
  visitor->Trace(selection_editor_);
  visitor->Trace(frame_caret_);
  SynchronousMutationObserver::Trace(visitor);
}
