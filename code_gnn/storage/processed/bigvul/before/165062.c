FontSelector* OffscreenCanvas::GetFontSelector() {
  if (auto* document = DynamicTo<Document>(GetExecutionContext())) {
    return document->GetStyleEngine().GetFontSelector();
  }
  return To<WorkerGlobalScope>(GetExecutionContext())->GetFontSelector();
}
