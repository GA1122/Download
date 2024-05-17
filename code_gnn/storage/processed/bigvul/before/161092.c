static bool TimeComparator(scoped_refptr<DevToolsAgentHost> host1,
                           scoped_refptr<DevToolsAgentHost> host2) {
  return host1->GetLastActivityTime() > host2->GetLastActivityTime();
}
