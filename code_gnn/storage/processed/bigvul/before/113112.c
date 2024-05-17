 ACTION_P(ScheduleRenameCallback, new_path) {
   BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                           base::Bind(arg3, new_path));
 }
