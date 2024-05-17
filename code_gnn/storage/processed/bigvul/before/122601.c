void LogSuccess(const Extension* extension,
                const std::string& api_name,
                scoped_ptr<ListValue> args,
                Profile* profile) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
    BrowserThread::PostTask(BrowserThread::UI,
                            FROM_HERE,
                            base::Bind(&LogSuccess,
                                       extension,
                                       api_name,
                                       base::Passed(&args),
                                       profile));
  } else {
    extensions::ActivityLog* activity_log =
        extensions::ActivityLog::GetInstance(profile);
    activity_log->LogAPIAction(extension, api_name, args.get(), "");
  }
}
