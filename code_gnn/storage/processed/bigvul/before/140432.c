ContentSuggestionsNotifierService::ContentSuggestionsNotifierService(
    Profile* profile,
    ContentSuggestionsService* suggestions)
    : observer_(base::MakeUnique<NotifyingObserver>(suggestions, profile)) {
  ContentSuggestionsNotificationHelper::FlushCachedMetrics();
  suggestions->AddObserver(observer_.get());
}
