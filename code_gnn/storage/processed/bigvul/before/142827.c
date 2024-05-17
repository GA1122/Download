bool HTMLMediaElement::LayoutObjectIsNeeded(const ComputedStyle& style) const {
  return ShouldShowControls() && HTMLElement::LayoutObjectIsNeeded(style);
}
