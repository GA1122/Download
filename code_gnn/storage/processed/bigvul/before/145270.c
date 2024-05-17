void DisplaySourceCustomBindings::Invalidate() {
  session_map_.clear();
  weak_factory_.InvalidateWeakPtrs();
  ObjectBackedNativeHandler::Invalidate();
}
