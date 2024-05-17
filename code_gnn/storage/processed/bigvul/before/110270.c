NaClListener::~NaClListener() {
  NOTREACHED();
  shutdown_event_.Signal();
#if defined(OS_WIN)
  g_listener = NULL;
#endif
}
