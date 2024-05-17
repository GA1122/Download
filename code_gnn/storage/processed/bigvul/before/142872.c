void HTMLMediaElement::Repaint() {
  if (cc_layer_)
    cc_layer_->SetNeedsDisplay();

  UpdateDisplayState();
  if (GetLayoutObject())
    GetLayoutObject()->SetShouldDoFullPaintInvalidation();
}
