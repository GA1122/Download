OomInterventionTabHelper::OomInterventionTabHelper(
    content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      decider_(OomInterventionDecider::GetForBrowserContext(
          web_contents->GetBrowserContext())),
      binding_(this),
      scoped_observer_(this),
      weak_ptr_factory_(this) {
  scoped_observer_.Add(crash_reporter::CrashMetricsReporter::GetInstance());
}
