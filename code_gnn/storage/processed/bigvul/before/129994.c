int ChromeBrowserMainParts::PreCreateThreads() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PreCreateThreads");
  result_code_ = PreCreateThreadsImpl();

  if (result_code_ == content::RESULT_CODE_NORMAL_EXIT) {
#if !defined(OS_ANDROID)
    DCHECK(master_prefs_.get());
    DCHECK(browser_creator_.get());
#endif   
    for (size_t i = 0; i < chrome_extra_parts_.size(); ++i)
      chrome_extra_parts_[i]->PreCreateThreads();
  }

  return result_code_;
}
