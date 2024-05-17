void LocalSiteCharacteristicsWebContentsObserver::WebContentsDestroyed() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!g_skip_observer_registration_for_testing) {
    TabLoadTracker::Get()->RemoveObserver(this);
    PageSignalReceiver::GetInstance()->RemoveObserver(this);
  }
  writer_.reset();
  writer_origin_ = url::Origin();
}
