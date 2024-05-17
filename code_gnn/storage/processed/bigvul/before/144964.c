void RenderWidgetHostViewAura::ParentHierarchyChanged() {
  ancestor_window_observer_.reset(new WindowAncestorObserver(this));
  HandleParentBoundsChanged();
}
