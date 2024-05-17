OfflinePageModelImpl::OfflinePageModelImpl()
    : OfflinePageModel(),
      is_loaded_(false),
      testing_clock_(nullptr),
      skip_clearing_original_url_for_testing_(false),
      weak_ptr_factory_(this) {}
