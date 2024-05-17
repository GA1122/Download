void RenderMessageFilter::OnGetMonitorColorProfile(std::vector<char>* profile) {
#if defined(OS_WIN)
  DCHECK(!BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (BackingStoreWin::ColorManagementEnabled())
    return;
#endif
  *profile = g_color_profile.Get().profile();
}
