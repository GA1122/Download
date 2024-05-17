ChromeContentBrowserClient::GetNetworkContextsParentDirectory() {
  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  DCHECK(!user_data_dir.empty());

  base::FilePath cache_dir;
  chrome::GetUserCacheDirectory(user_data_dir, &cache_dir);
  DCHECK(!cache_dir.empty());

  if (user_data_dir.IsParent(cache_dir))
    return {user_data_dir};

  return {user_data_dir, cache_dir};
}
