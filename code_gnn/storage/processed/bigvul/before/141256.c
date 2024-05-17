static Element* SkipDisplayNoneAncestorsOrReturnNullIfFlatTreeIsDirty(
    Element& element) {
  if (element.GetDocument().IsSlotAssignmentOrLegacyDistributionDirty()) {
    return nullptr;
  }
  return SkipDisplayNoneAncestors(&element);
}
