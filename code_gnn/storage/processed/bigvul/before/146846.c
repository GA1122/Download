RefPtr<ComputedStyle> Document::StyleForElementIgnoringPendingStylesheets(
    Element* element) {
  DCHECK_EQ(element->GetDocument(), this);
  StyleEngine::IgnoringPendingStylesheet ignoring(GetStyleEngine());
  if (!element->CanParticipateInFlatTree())
    return EnsureStyleResolver().StyleForElement(element, nullptr);

  ContainerNode* parent = LayoutTreeBuilderTraversal::Parent(*element);
  const ComputedStyle* parent_style =
      parent ? parent->EnsureComputedStyle() : nullptr;

  ContainerNode* layout_parent =
      parent ? LayoutTreeBuilderTraversal::LayoutParent(*element) : nullptr;
  const ComputedStyle* layout_parent_style =
      layout_parent ? layout_parent->EnsureComputedStyle() : parent_style;

  return EnsureStyleResolver().StyleForElement(element, parent_style,
                                               layout_parent_style);
}
