bool BaseSessionService::RunTaskOnBackendThread(
    const tracked_objects::Location& from_here,
    const base::Closure& task) {
  if (RunningInProduction()) {
    return BrowserThread::PostTask(BrowserThread::FILE, from_here, task);
  } else {
    task.Run();
     return true;
   }
 }
