AutofillDownloadManager::AutofillDownloadManager(Profile* profile)
    : profile_(profile),
      observer_(NULL),
      max_form_cache_size_(kMaxFormCacheSize),
      next_query_request_(base::Time::Now()),
      next_upload_request_(base::Time::Now()),
      positive_upload_rate_(0),
      negative_upload_rate_(0),
      fetcher_id_for_unittest_(0) {
  if (profile_) {
    PrefService* preferences = profile_->GetPrefs();
    positive_upload_rate_ =
        preferences->GetDouble(prefs::kAutofillPositiveUploadRate);
    negative_upload_rate_ =
        preferences->GetDouble(prefs::kAutofillNegativeUploadRate);
  }
}
