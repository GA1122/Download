void HTMLCanvasElement::OnContentsCcLayerChanged() {
  if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled() &&
      GetLayoutObject() && GetLayoutObject()->HasLayer())
    GetLayoutBoxModelObject()->Layer()->SetNeedsRepaint();

  if (auto* layer = ContentsCcLayer())
    layer->set_owner_node_id(DOMNodeIds::IdForNode(this));
}
