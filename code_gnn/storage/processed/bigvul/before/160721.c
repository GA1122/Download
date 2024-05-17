void RenderFrameImpl::ScriptedPrint(bool user_initiated) {
  SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.ScriptedPrint");
  for (auto& observer : observers_)
    observer.ScriptedPrint(user_initiated);
}
