void BaseRenderingContext2D::setGlobalCompositeOperation(
    const String& operation) {
  CompositeOperator op = kCompositeSourceOver;
  WebBlendMode blend_mode = WebBlendMode::kNormal;
  if (!ParseCompositeAndBlendOperator(operation, op, blend_mode))
    return;
  SkBlendMode xfermode = WebCoreCompositeToSkiaComposite(op, blend_mode);
  if (GetState().GlobalComposite() == xfermode)
    return;
  ModifiableState().SetGlobalComposite(xfermode);
}
