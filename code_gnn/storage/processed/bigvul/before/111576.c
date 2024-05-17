  GDataFileSystemTest()
      : ui_thread_(content::BrowserThread::UI, &message_loop_),
        io_thread_(content::BrowserThread::IO),
        file_system_(NULL),
        mock_doc_service_(NULL),
        num_callback_invocations_(0),
        expected_error_(base::PLATFORM_FILE_OK),
        expected_cache_state_(0),
        expected_sub_dir_type_(GDataRootDirectory::CACHE_TYPE_META),
        expect_outgoing_symlink_(false),
        root_feed_changestamp_(0) {
  }
