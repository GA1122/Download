void Browser::CloseFrameAfterDragSession() {
#if !defined(OS_MACOSX)
  MessageLoop::current()->PostTask(
      FROM_HERE, method_factory_.NewRunnableMethod(&Browser::CloseFrame));
#endif
}
