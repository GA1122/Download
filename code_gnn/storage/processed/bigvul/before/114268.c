void ClearSharedContexts() {
  base::AutoLock lock(g_all_shared_contexts_lock.Get());
  g_all_shared_contexts.Pointer()->clear();
}
