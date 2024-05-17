void TestingAutomationProvider::KillRendererProcess(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  int pid;
  if (!args->GetInteger("pid", &pid)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'pid' key missing or invalid.");
    return;
  }
  base::ProcessHandle process;
  if (!base::OpenProcessHandle(static_cast<base::ProcessId>(pid), &process)) {
    AutomationJSONReply(this, reply_message).SendError(base::StringPrintf(
        "Failed to open process handle for pid %d", pid));
    return;
  }
  new RendererProcessClosedObserver(this, reply_message);
  base::KillProcess(process, 0, false);
  base::CloseProcessHandle(process);
}
