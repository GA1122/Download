FrameOrWorkerScheduler* Document::GetScheduler() {
  DCHECK(IsMainThread());

  if (ContextDocument() && ContextDocument()->GetFrame())
    return ContextDocument()->GetFrame()->GetFrameScheduler();
  return nullptr;
}
