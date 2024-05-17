void ChildThread::ShutdownThread() {
  DCHECK(!ChildThread::current()) <<
      "this method should NOT be called from child thread itself";
  {
    base::AutoLock lock(g_lazy_child_thread_lock.Get());
    while (!g_child_thread)
      g_lazy_child_thread_cv.Get().Wait();
  }
  DCHECK_NE(base::MessageLoop::current(), g_child_thread->message_loop());
  g_child_thread->message_loop()->PostTask(
      FROM_HERE, base::Bind(&QuitMainThreadMessageLoop));
}
