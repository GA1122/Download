bool Core::AddDispatchersFromTransit(
    const std::vector<Dispatcher::DispatcherInTransit>& dispatchers,
    MojoHandle* handles) {
  bool failed = false;
  {
    base::AutoLock lock(handles_->GetLock());
    if (!handles_->AddDispatchersFromTransit(dispatchers, handles))
      failed = true;
  }
  if (failed) {
    for (auto d : dispatchers) {
      if (d.dispatcher)
        d.dispatcher->Close();
    }
    return false;
  }
  return true;
}
