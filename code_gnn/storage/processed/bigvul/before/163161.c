void HTMLIFrameElement::SetCollapsed(bool collapse) {
  if (collapsed_by_client_ == collapse)
    return;

  collapsed_by_client_ = collapse;

  DCHECK(!GetDocument().InStyleRecalc());
  LazyReattachIfAttached();
}
