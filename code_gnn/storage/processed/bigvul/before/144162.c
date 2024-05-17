bool IsUsingWMPointerForTouch() {
  return base::win::GetVersion() >= base::win::VERSION_WIN8 &&
         base::FeatureList::IsEnabled(kPointerEventsForTouch);
}
