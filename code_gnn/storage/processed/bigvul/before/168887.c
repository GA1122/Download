bool DebuggerGetTargetsFunction::RunAsync() {
  content::DevToolsAgentHost::List list = DevToolsAgentHost::GetOrCreateAll();
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(&DebuggerGetTargetsFunction::SendTargetList, this, list));
  return true;
}
