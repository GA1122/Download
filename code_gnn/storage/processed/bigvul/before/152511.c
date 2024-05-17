void RenderFrameImpl::ScriptedPrint(bool user_initiated) {
  for (auto& observer : observers_)
    observer.ScriptedPrint(user_initiated);
}
