const base::string16& TitleWatcher::WaitAndGetTitle() {
  TestTitle();
  run_loop_.Run();
  return observed_title_;
}
