void Document::EnsurePaintLocationDataValidForNode(const Node* node) {
  DCHECK(node);
  if (!node->InActiveDocument())
    return;

  UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (View() && node->GetLayoutObject() &&
      node->GetLayoutObject()->StyleRef().SubtreeIsSticky()) {
    bool success = false;
    if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
      success = View()->UpdateAllLifecyclePhasesExceptPaint();
    } else {
      success = View()->UpdateLifecycleToCompositingInputsClean();
    }
    DCHECK(success);
  }
}
