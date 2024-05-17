NaClProcessHost::NaClProcessHost(const std::wstring& url)
    : BrowserChildProcessHost(NACL_LOADER_PROCESS),
      reply_msg_(NULL),
      internal_(new NaClInternal()),
      running_on_wow64_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(callback_factory_(this)) {
  set_name(WideToUTF16Hack(url));
#if defined(OS_WIN)
  running_on_wow64_ = (base::win::OSInfo::GetInstance()->wow64_status() ==
      base::win::OSInfo::WOW64_ENABLED);
#endif
}
