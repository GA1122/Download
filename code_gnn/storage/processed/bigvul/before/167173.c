void HTMLMediaElement::Repaint() {
  if (web_layer_)
    web_layer_->Invalidate();

  UpdateDisplayState();
  if (GetLayoutObject())
    GetLayoutObject()->SetShouldDoFullPaintInvalidation();
}
