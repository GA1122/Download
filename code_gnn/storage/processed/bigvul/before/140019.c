bool HTMLMediaElement::layoutObjectIsNeeded(const ComputedStyle& style) {
  return shouldShowControls() && HTMLElement::layoutObjectIsNeeded(style);
}
