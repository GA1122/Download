void LayerTreeHost::SetElementFilterMutated(ElementId element_id,
                                            ElementListType list_type,
                                            const FilterOperations& filters) {
  Layer* layer = LayerByElementId(element_id);
  DCHECK(layer);
  layer->OnFilterAnimated(filters);
}
