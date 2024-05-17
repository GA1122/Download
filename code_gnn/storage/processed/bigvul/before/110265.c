NaClListener::NaClListener() : shutdown_event_(true, false),
                               io_thread_("NaCl_IOThread"),
                               main_loop_(NULL) {
  io_thread_.StartWithOptions(base::Thread::Options(MessageLoop::TYPE_IO, 0));
#if defined(OS_WIN)
  DCHECK(g_listener == NULL);
  g_listener = this;
#endif
}
