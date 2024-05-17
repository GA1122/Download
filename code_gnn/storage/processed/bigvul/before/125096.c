void ChromePluginServiceFilter::RestrictPluginToProfileAndOrigin(
    const FilePath& plugin_path,
    Profile* profile,
    const GURL& origin) {
  base::AutoLock auto_lock(lock_);
  restricted_plugins_[plugin_path] =
      std::make_pair(PluginPrefs::GetForProfile(profile),
                     origin);
}
