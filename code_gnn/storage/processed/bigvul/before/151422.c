WebFrameScheduler* FrameFetchContext::GetFrameScheduler() {
  if (IsDetached())
    return nullptr;
  return GetFrame()->FrameScheduler();
}
