  static std::unique_ptr<Task> Create(ImageLoader* loader,
                                      UpdateFromElementBehavior update_behavior,
                                      ReferrerPolicy referrer_policy) {
    return std::make_unique<Task>(loader, update_behavior, referrer_policy);
  }
