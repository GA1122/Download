void BaseRenderingContext2D::setShadowColor(const String& color_string) {
  Color color;
  if (!ParseColorOrCurrentColor(color, color_string))
    return;
  if (GetState().ShadowColor() == color)
    return;
  ModifiableState().SetShadowColor(color.Rgb());
}
