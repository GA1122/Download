void GDataFileSystem::StartUpdates() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DCHECK(!update_timer_.IsRunning());
  update_timer_.Start(FROM_HERE,
                      base::TimeDelta::FromSeconds(
                          kGDataUpdateCheckIntervalInSec),
                      base::Bind(&GDataFileSystem::CheckForUpdates,
                          ui_weak_ptr_));
}
