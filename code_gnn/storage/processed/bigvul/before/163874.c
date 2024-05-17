void DevToolsWindow::InspectElement(
    content::RenderFrameHost* inspected_frame_host,
    int x,
    int y) {
  WebContents* web_contents =
      WebContents::FromRenderFrameHost(inspected_frame_host);
  scoped_refptr<DevToolsAgentHost> agent(
      DevToolsAgentHost::GetOrCreateFor(web_contents));
  bool should_measure_time = FindDevToolsWindow(agent.get()) == NULL;
  base::TimeTicks start_time = base::TimeTicks::Now();
  OpenDevToolsWindow(web_contents, DevToolsToggleAction::ShowElementsPanel());
  DevToolsWindow* window = FindDevToolsWindow(agent.get());
  if (window) {
    agent->InspectElement(window->bindings_, x, y);
    if (should_measure_time)
      window->inspect_element_start_time_ = start_time;
  }
}
