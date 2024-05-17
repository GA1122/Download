void SVGElement::MarkForLayoutAndParentResourceInvalidation(
    LayoutObject& layout_object) {
  LayoutSVGResourceContainer::MarkForLayoutAndParentResourceInvalidation(
      layout_object, true);
}
