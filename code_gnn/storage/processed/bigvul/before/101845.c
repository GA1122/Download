void Browser::TabStripEmpty() {
  MessageLoop::current()->PostTask(
      FROM_HERE, method_factory_.NewRunnableMethod(&Browser::CloseFrame));
}
