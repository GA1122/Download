void HTMLCanvasElement::ContextDestroyed(ExecutionContext*) {
  if (context_)
    context_->Stop();
}
