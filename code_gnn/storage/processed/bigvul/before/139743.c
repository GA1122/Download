ResourceFetcher::ResourceFetcher(FetchContext* new_context)
    : context_(new_context),
      archive_(Context().IsMainFrame() ? nullptr : Context().Archive()),
      resource_timing_report_timer_(
          Context().LoadingTaskRunner()
              ? Context().LoadingTaskRunner()
              : Platform::Current()->CurrentThread()->GetWebTaskRunner(),
          this,
          &ResourceFetcher::ResourceTimingReportTimerFired),
      auto_load_images_(true),
      images_enabled_(true),
      allow_stale_resources_(false),
      image_fetched_(false) {}