void SystemKeyEventListener::Stop() {
  if (stopped_)
    return;
#if defined(TOOLKIT_USES_GTK)
  gdk_window_remove_filter(NULL, GdkEventFilter, this);
#else
  MessageLoopForUI::current()->RemoveObserver(this);
#endif
  stopped_ = true;
}
