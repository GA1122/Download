void Browser::CloseFrameAfterDragSession() {
#if defined(OS_WIN) || defined(OS_LINUX)
  MessageLoop::current()->PostTask(
      FROM_HERE, method_factory_.NewRunnableMethod(&Browser::CloseFrame));
#endif
}
