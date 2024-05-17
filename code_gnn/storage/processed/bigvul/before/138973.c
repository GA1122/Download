void WallpaperManagerBase::MoveLoggedInUserCustomWallpaper() {
  DCHECK(thread_checker_.CalledOnValidThread());
  const user_manager::User* logged_in_user =
      user_manager::UserManager::Get()->GetActiveUser();
  if (logged_in_user) {
    task_runner_->PostTask(
        FROM_HERE,
        base::Bind(&WallpaperManagerBase::MoveCustomWallpapersOnWorker,
                   logged_in_user->GetAccountId(),
                   GetFilesId(logged_in_user->GetAccountId()),
                   base::ThreadTaskRunnerHandle::Get(),
                   weak_factory_.GetWeakPtr()));
  }
}
