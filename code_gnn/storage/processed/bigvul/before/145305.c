void RenderFrameObserverNatives::Invalidate() {
  weak_ptr_factory_.InvalidateWeakPtrs();
  ObjectBackedNativeHandler::Invalidate();
}
