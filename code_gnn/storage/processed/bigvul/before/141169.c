void Document::NodeWillBeRemoved(Node& n) {
  for (NodeIterator* ni : node_iterators_)
    ni->NodeWillBeRemoved(n);

  for (Range* range : ranges_) {
    range->NodeWillBeRemoved(n);
    if (range == sequential_focus_navigation_starting_point_)
      range->FixupRemovedNodeAcrossShadowBoundary(n);
  }

  NotifyNodeWillBeRemoved(n);

  if (ContainsV1ShadowTree())
    n.CheckSlotChangeBeforeRemoved();

  if (n.InActiveDocument())
    GetStyleEngine().NodeWillBeRemoved(n);
}
