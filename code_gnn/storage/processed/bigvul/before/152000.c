void LogRendererKillCrashKeys(const GURL& site_url) {
  static auto* site_url_key = base::debug::AllocateCrashKeyString(
      "current_site_url", base::debug::CrashKeySize::Size64);
  base::debug::SetCrashKeyString(site_url_key, site_url.spec());
}
