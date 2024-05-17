  MockRenderProcessHostWithSignaling(BrowserContext* context,
                                     base::RunLoop* auth_run_loop)
      : MockRenderProcessHost(context), auth_run_loop_(auth_run_loop) {}
