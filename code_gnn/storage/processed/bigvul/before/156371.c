bool DebuggerGetTargetsFunction::RunAsync() {
  content::DevToolsAgentHost::List list = DevToolsAgentHost::GetOrCreateAll();
  base::PostTaskWithTraits(
      FROM_HERE, {content::BrowserThread::UI},
      base::BindOnce(&DebuggerGetTargetsFunction::SendTargetList, this, list));
  return true;
}
