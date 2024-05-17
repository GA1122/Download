void Document::EnsurePaintLocationDataValidForNode(const Node* node) {
  DCHECK(node);
  if (!node->InActiveDocument())
    return;

  UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (View() && node->GetLayoutObject() &&
      node->GetLayoutObject()->StyleRef().SubtreeIsSticky()) {
    if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
      View()->UpdateAllLifecyclePhasesExceptPaint();
    } else {
      View()->UpdateLifecycleToCompositingInputsClean();
    }
  }
}
