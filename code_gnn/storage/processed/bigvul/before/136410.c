PaintArtifactCompositor::~PaintArtifactCompositor() {
  if (RuntimeEnabledFeatures::BlinkGenPropertyTreesEnabled())
    return;
  for (auto child : root_layer_->children())
    DCHECK(!child->element_id());
}
