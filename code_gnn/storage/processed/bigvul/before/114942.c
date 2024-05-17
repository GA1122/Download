void TestingAutomationProvider::GetProcessInfo(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  scoped_refptr<ProcessInfoObserver>
      proc_observer(new ProcessInfoObserver(this, reply_message));
  proc_observer->StartFetch();
}
