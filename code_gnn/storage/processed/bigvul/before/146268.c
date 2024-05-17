unsigned CurrentMaxGLContexts() {
  return IsMainThread() ? kMaxGLActiveContexts : kMaxGLActiveContextsOnWorker;
}
