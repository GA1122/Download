int RunTests(const scoped_ptr<content::BrowserMainRunner>& main_runner) {
  content::WebKitTestController test_controller;
  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    base::FilePath temp_path;
    base::GetTempDir(&temp_path);
    test_controller.SetTempPath(temp_path);
  }
  std::string test_string;
  base::CommandLine::StringVector args =
      base::CommandLine::ForCurrentProcess()->GetArgs();
  size_t command_line_position = 0;
  bool ran_at_least_once = false;

  std::cout << "#READY\n";
  std::cout.flush();

  while (GetNextTest(args, &command_line_position, &test_string)) {
    if (!RunOneTest(test_string, &ran_at_least_once, main_runner))
      break;
  }
  if (!ran_at_least_once) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    main_runner->Run();
  }

#if defined(OS_ANDROID)
  main_runner->Shutdown();
#endif

  return 0;
}
