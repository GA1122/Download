NTPResourceCache::WindowType NTPResourceCache::GetWindowType(
    Profile* profile, content::RenderProcessHost* render_host) {
  if (profile->IsGuestSession()) {
    return NTPResourceCache::GUEST;
  } else if (render_host) {
    if (render_host->GetBrowserContext()->IsOffTheRecord())
      return NTPResourceCache::INCOGNITO;
  } else if (profile->IsOffTheRecord()) {
    return NTPResourceCache::INCOGNITO;
  }
  return NTPResourceCache::NORMAL;
}
