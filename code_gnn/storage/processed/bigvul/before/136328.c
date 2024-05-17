void FragmentPaintPropertyTreeBuilder::UpdateForChildren() {
#if DCHECK_IS_ON()
  FindObjectPropertiesNeedingUpdateScope check_needs_update_scope(
      object_, fragment_data_, full_context_.force_subtree_update);
#endif

  if (properties_) {
    UpdateInnerBorderRadiusClip();
    UpdateOverflowClip();
    UpdatePerspective();
    UpdateSvgLocalToBorderBoxTransform();
    UpdateScrollAndScrollTranslation();
  }
  UpdateOutOfFlowContext();
}
