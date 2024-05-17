void HTMLCanvasElement::NotifyGpuContextLost() {
  if (Is2d())
    context_->LoseContext(CanvasRenderingContext::kRealLostContext);
}
