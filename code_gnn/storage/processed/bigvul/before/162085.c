RenderProcessHost* RenderProcessHost::FromRendererIdentity(
    const service_manager::Identity& identity) {
  for (content::RenderProcessHost::iterator i(
           content::RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance()) {
    content::RenderProcessHost* process = i.GetCurrentValue();
    if (process->GetChildIdentity() == identity)
      return process;
  }
  return nullptr;
}
