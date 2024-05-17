void BaseRenderingContext2D::setTextBaseline(const String& s) {
  TextBaseline baseline;
  if (!ParseTextBaseline(s, baseline))
    return;
  if (GetState().GetTextBaseline() == baseline)
    return;
  ModifiableState().SetTextBaseline(baseline);
}
