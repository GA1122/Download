void LogFailure(const Extension* extension,
                const std::string& api_name,
                scoped_ptr<ListValue> args,
                const char* reason,
                Profile* profile) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
    BrowserThread::PostTask(BrowserThread::UI,
                            FROM_HERE,
                            base::Bind(&LogFailure,
                                       extension,
                                       api_name,
                                       base::Passed(&args),
                                       reason,
                                       profile));
  } else {
    extensions::ActivityLog* activity_log =
        extensions::ActivityLog::GetInstance(profile);
    activity_log->LogBlockedAction(extension,
                                   api_name,
                                   args.get(),
                                   reason,
                                   "");
  }
}
