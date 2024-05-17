void SupervisedUserService::LoadBlacklist(const base::FilePath& path,
                                          const GURL& url) {
  DCHECK(blacklist_state_ == BlacklistLoadState::NOT_LOADED);
  blacklist_state_ = BlacklistLoadState::LOAD_STARTED;
  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE,
      {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::BindOnce(&base::PathExists, path),
      base::BindOnce(&SupervisedUserService::OnBlacklistFileChecked,
                     weak_ptr_factory_.GetWeakPtr(), path, url));
}
