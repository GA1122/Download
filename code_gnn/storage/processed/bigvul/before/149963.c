bool LayerTreeHostImpl::IsElementInList(ElementId element_id,
                                        ElementListType list_type) const {
  if (list_type == ElementListType::ACTIVE) {
    return active_tree()
               ? active_tree()->LayerByElementId(element_id) != nullptr
               : false;
  } else {
    if (pending_tree() && pending_tree()->LayerByElementId(element_id))
      return true;
    if (recycle_tree() && recycle_tree()->LayerByElementId(element_id))
      return true;

    return false;
  }
}
