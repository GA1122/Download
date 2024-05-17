DEFINE_TRACE(SelectionEditor) {
  visitor->Trace(frame_);
  visitor->Trace(selection_);
  visitor->Trace(cached_visible_selection_in_dom_tree_);
  visitor->Trace(cached_visible_selection_in_flat_tree_);
  visitor->Trace(cached_range_);
  SynchronousMutationObserver::Trace(visitor);
}
