  ProfileSyncServiceSessionTest()
      : io_thread_(BrowserThread::IO),
        window_bounds_(0, 1, 2, 3),
        notified_of_update_(false),
        notified_of_refresh_(false) {}
