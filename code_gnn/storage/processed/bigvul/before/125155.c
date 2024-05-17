void WillLoadPluginsCallback(
    base::SequencedWorkerPool::SequenceToken token) {
  if (LoadPluginListInProcess()) {
    CHECK(BrowserThread::GetBlockingPool()->IsRunningSequenceOnCurrentThread(
        token));
  } else {
    CHECK(false) << "Plugin loading should happen out-of-process.";
  }
}
