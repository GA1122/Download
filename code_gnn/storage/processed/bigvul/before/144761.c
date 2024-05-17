void LocalSiteCharacteristicsWebContentsObserver::OnVisibilityChanged(
    content::Visibility visibility) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!writer_)
    return;

  auto rc_visibility = ContentVisibilityToRCVisibility(visibility);
  writer_->NotifySiteVisibilityChanged(rc_visibility);
}
