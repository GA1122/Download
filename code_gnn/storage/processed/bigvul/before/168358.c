bool BrowserView::ShouldUseImmersiveFullscreenForUrl(const GURL& url) const {
#if defined(OS_CHROMEOS)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    return false;

  if (profiles::IsPublicSession())
    return true;

  return url.is_empty();
#else
  return false;
#endif
}
