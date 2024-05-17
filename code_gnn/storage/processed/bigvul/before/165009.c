bool HTMLCanvasElement::HasImageBitmapContext() const {
  if (!context_)
    return false;
  CanvasRenderingContext::ContextType type = context_->GetContextType();
  return (type == CanvasRenderingContext::kContextImageBitmap ||
          type == CanvasRenderingContext::kContextXRPresent);
}
