bool HTMLCanvasElement::PaintsIntoCanvasBuffer() const {
  if (PlaceholderFrame())
    return false;
  DCHECK(context_);
  if (!context_->IsComposited())
    return true;
  if (GetLayoutBox() && GetLayoutBox()->HasAcceleratedCompositing())
    return false;

  return true;
}
