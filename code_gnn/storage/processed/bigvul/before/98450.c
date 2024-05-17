void SearchProviderTest::RunTillProviderDone() {
  if (provider_->done())
    return;

  quit_when_done_ = true;
#if defined(OS_MACOSX)
  message_loop_.Run();
#else
  message_loop_.Run(NULL);
#endif
}
