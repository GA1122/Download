void StartupTimeBomb::DisarmStartupTimeBomb() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (g_startup_timebomb_)
    g_startup_timebomb_->Disarm();
}
