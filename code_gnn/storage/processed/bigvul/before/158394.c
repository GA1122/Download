  MockRenderProcessHost* CreateNewProcessHost() {
    MockRenderProcessHost* process_host =
        new MockRenderProcessHost(browser_context());
    return process_host;
  }
