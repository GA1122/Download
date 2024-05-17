bool LayerTreeHost::IsElementInList(ElementId element_id,
                                    ElementListType list_type) const {
  return list_type == ElementListType::ACTIVE && LayerByElementId(element_id);
}
