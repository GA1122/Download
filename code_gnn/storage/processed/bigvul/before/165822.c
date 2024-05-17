bool SVGElement::SendSVGLoadEventIfPossible() {
  if (!HaveLoadedRequiredResources())
    return false;
  if ((IsStructurallyExternal() || IsSVGSVGElement(*this)) &&
      HasLoadListener(this))
    DispatchEvent(*Event::Create(event_type_names::kLoad));
  return true;
}
