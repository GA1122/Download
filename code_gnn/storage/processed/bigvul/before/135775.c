DEFINE_TRACE(SelectionController) {
  visitor->Trace(frame_);
  visitor->Trace(original_base_in_flat_tree_);
  SynchronousMutationObserver::Trace(visitor);
}
