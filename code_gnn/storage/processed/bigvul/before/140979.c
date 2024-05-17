bool Document::ChildrenCanHaveStyle() const {
  if (LayoutObject* view = GetLayoutView())
    return view->CanHaveChildren();
  return false;
}
