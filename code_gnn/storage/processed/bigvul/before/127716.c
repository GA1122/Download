  ExtensionWebRequestHeaderModificationTest()
      : thread_bundle_(content::TestBrowserThreadBundle::IO_MAINLOOP),
        profile_manager_(TestingBrowserProcess::GetGlobal()),
        event_router_(new EventRouterForwarder) {}
