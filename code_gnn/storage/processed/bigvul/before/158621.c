FrameScheduler* FrameFetchContext::GetFrameScheduler() const {
  if (IsDetached())
    return nullptr;
  return GetFrame()->GetFrameScheduler();
}
