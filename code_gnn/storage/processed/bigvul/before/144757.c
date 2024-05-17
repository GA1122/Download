    MaybeNotifyBackgroundFeatureUsage(
        void (SiteCharacteristicsDataWriter::*method)()) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (ShouldIgnoreFeatureUsageEvent())
    return;

  if (ContentVisibilityToRCVisibility(web_contents()->GetVisibility()) ==
      TabVisibility::kBackground) {
    (writer_.get()->*method)();
  }
}
