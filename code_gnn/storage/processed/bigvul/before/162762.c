void BaseRenderingContext2D::setLineCap(const String& s) {
  LineCap cap;
  if (!ParseLineCap(s, cap))
    return;
  if (GetState().GetLineCap() == cap)
    return;
  ModifiableState().SetLineCap(cap);
}
