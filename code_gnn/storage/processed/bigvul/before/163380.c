int RenderThreadImpl::PostTaskToAllWebWorkers(const base::Closure& closure) {
  return WorkerThreadRegistry::Instance()->PostTaskToAllThreads(closure);
}
