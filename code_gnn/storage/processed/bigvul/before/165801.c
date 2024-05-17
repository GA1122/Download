void SVGElement::InvalidateRelativeLengthClients(
    SubtreeLayoutScope* layout_scope) {
  if (!isConnected())
    return;

#if DCHECK_IS_ON()
  DCHECK(!in_relative_length_clients_invalidation_);
  base::AutoReset<bool> in_relative_length_clients_invalidation_change(
      &in_relative_length_clients_invalidation_, true);
#endif

  if (LayoutObject* layout_object = this->GetLayoutObject()) {
    if (HasRelativeLengths() && layout_object->IsSVGResourceContainer()) {
      ToLayoutSVGResourceContainer(layout_object)
          ->InvalidateCacheAndMarkForLayout(
              layout_invalidation_reason::kSizeChanged, layout_scope);
    } else if (SelfHasRelativeLengths()) {
      layout_object->SetNeedsLayoutAndFullPaintInvalidation(
          layout_invalidation_reason::kUnknown, kMarkContainerChain,
          layout_scope);
    }
  }

  for (SVGElement* element : elements_with_relative_lengths_) {
    if (element != this)
      element->InvalidateRelativeLengthClients(layout_scope);
  }
}
