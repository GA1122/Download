bool CSSPaintValue::Equals(const CSSPaintValue& other) const {
  return GetName() == other.GetName() &&
         CustomCSSText() == other.CustomCSSText();
}
