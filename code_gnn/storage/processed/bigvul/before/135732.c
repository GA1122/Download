DEFINE_TRACE(InputMethodController) {
  visitor->Trace(frame_);
  visitor->Trace(composition_range_);
  SynchronousMutationObserver::Trace(visitor);
}
