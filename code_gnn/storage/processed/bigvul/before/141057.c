void Document::EnsurePaintLocationDataValidForNode(const Node* node) {
  DCHECK(node);
  if (!node->InActiveDocument())
    return;

  DisplayLockUtilities::ScopedChainForcedUpdate scoped_update_forced(node);

  UpdateStyleAndLayout();

  if (View() && node->GetLayoutObject() &&
      node->GetLayoutObject()->StyleRef().SubtreeIsSticky()) {
    bool success = false;
    if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled()) {
      success = View()->UpdateAllLifecyclePhasesExceptPaint();
    } else {
      success = View()->UpdateLifecycleToCompositingInputsClean();
    }
    DCHECK(success);
  }
}
