bool HTMLCanvasElement::OriginClean() const {
  if (GetDocument().GetSettings() &&
      GetDocument().GetSettings()->GetDisableReadingFromCanvas()) {
    return false;
  }
  if (PlaceholderFrame())
    return PlaceholderFrame()->OriginClean();
  return origin_clean_;
}
