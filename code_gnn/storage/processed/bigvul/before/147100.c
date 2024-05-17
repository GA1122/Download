SingleThreadTaskRunner* WebLocalFrameImpl::TimerTaskRunner() {
  return GetFrame()
      ->FrameScheduler()
      ->TimerTaskRunner()
      ->ToSingleThreadTaskRunner();
}
