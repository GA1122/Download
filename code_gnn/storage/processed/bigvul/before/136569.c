const CompositorElementId RefCountedPropertyTreeState::GetCompositorElementId(
    const CompositorElementIdSet& element_ids) const {
  if (Effect()->GetCompositorElementId() &&
      !element_ids.Contains(Effect()->GetCompositorElementId()))
    return Effect()->GetCompositorElementId();
  if (Transform()->GetCompositorElementId() &&
      !element_ids.Contains(Transform()->GetCompositorElementId()))
    return Transform()->GetCompositorElementId();
  return CompositorElementId();
}
