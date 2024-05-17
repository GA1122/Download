Color Document::ThemeColor() const {
  auto* root_element = documentElement();
  if (!root_element)
    return Color();
  for (HTMLMetaElement& meta_element :
       Traversal<HTMLMetaElement>::DescendantsOf(*root_element)) {
    Color color = Color::kTransparent;
    if (DeprecatedEqualIgnoringCase(meta_element.GetName(), "theme-color") &&
        CSSParser::ParseColor(
            color, meta_element.Content().GetString().StripWhiteSpace(), true))
      return color;
  }
  return Color();
}
