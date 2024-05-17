BaseAudioContext::~BaseAudioContext() {
  {
    GraphAutoLocker locker(this);
    destination_handler_ = nullptr;
  }

  GetDeferredTaskHandler().ContextWillBeDestroyed();
}
