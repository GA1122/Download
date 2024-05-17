InProcessBrowserTest::InProcessBrowserTest()
    : browser_(NULL),
      show_window_(false),
      dom_automation_enabled_(false),
      tab_closeable_state_watcher_enabled_(false) {
#if defined(OS_MACOSX)
  FilePath chrome_path;
  CHECK(PathService::Get(base::FILE_EXE, &chrome_path));
  chrome_path = chrome_path.DirName();
  chrome_path = chrome_path.Append(chrome::kBrowserProcessExecutablePath);
  CHECK(PathService::Override(base::FILE_EXE, chrome_path));
#endif   

  test_server_.reset(new net::TestServer(
      net::TestServer::TYPE_HTTP,
      FilePath(FILE_PATH_LITERAL("chrome/test/data"))));
}
