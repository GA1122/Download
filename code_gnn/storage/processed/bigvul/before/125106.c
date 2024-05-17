  PluginInfoMessageFilterTest() :
      foo_plugin_path_(FILE_PATH_LITERAL("/path/to/foo")),
      bar_plugin_path_(FILE_PATH_LITERAL("/path/to/bar")),
      file_thread_(content::BrowserThread::FILE, &message_loop_) {
  }
