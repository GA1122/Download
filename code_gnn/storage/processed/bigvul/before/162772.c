void BaseRenderingContext2D::setTextAlign(const String& s) {
  TextAlign align;
  if (!ParseTextAlign(s, align))
    return;
  if (GetState().GetTextAlign() == align)
    return;
  ModifiableState().SetTextAlign(align);
}
