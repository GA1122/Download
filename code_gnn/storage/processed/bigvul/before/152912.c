void DevToolsWindow::InspectElement(
    content::RenderFrameHost* inspected_frame_host,
    int x,
    int y) {
  scoped_refptr<DevToolsAgentHost> agent(
      DevToolsAgentHost::GetOrCreateFor(inspected_frame_host));
  bool should_measure_time = FindDevToolsWindow(agent.get()) == NULL;
  base::TimeTicks start_time = base::TimeTicks::Now();
  if (agent->GetType() == content::DevToolsAgentHost::kTypePage) {
    OpenDevToolsWindow(agent->GetWebContents());
  } else {
    OpenDevToolsWindowForFrame(Profile::FromBrowserContext(
                                   agent->GetBrowserContext()), agent);
  }
  DevToolsWindow* window = FindDevToolsWindow(agent.get());
  if (window) {
    agent->InspectElement(window->bindings_, x, y);
    if (should_measure_time)
      window->inspect_element_start_time_ = start_time;
  }
}
