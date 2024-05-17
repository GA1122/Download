void IOThread::SetGlobalsForTesting(Globals* globals) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(!globals || !globals_);
  globals_ = globals;
}
