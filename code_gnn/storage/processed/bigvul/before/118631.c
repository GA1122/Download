  LoginUtilsTest()
      : fake_io_thread_completion_(false, false),
        fake_io_thread_("fake_io_thread"),
        browser_process_(TestingBrowserProcess::GetGlobal()),
        local_state_(browser_process_),
        ui_thread_(BrowserThread::UI, &loop_),
        db_thread_(BrowserThread::DB, &loop_),
        file_thread_(BrowserThread::FILE, &loop_),
        mock_input_method_manager_(NULL),
        mock_async_method_caller_(NULL),
        connector_(NULL),
        prepared_profile_(NULL) {}
