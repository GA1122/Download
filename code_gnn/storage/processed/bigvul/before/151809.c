BackgroundLoaderOfflinerTest::BackgroundLoaderOfflinerTest()
    : thread_bundle_(content::TestBrowserThreadBundle::IO_MAINLOOP),
      load_termination_listener_(nullptr),
      model_(nullptr),
      completion_callback_called_(false),
      cancel_callback_called_(false),
      can_download_callback_called_(false),
      can_download_(false),
      progress_(0LL),
      request_status_(Offliner::RequestStatus::UNKNOWN) {}
