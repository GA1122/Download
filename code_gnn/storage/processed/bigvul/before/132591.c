void WebKitTestController::PluginCrashed(const base::FilePath& plugin_path,
                                         base::ProcessId plugin_pid) {
  DCHECK(CalledOnValidThread());
  printer_->AddErrorMessage(
      base::StringPrintf("#CRASHED - plugin (pid %d)", plugin_pid));
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(base::IgnoreResult(&WebKitTestController::DiscardMainWindow),
                 base::Unretained(this)));
}
