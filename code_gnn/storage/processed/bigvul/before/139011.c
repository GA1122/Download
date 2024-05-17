  static std::unique_ptr<Task> create(ImageLoader* loader,
                                      UpdateFromElementBehavior updateBehavior,
                                      ReferrerPolicy referrerPolicy) {
    return WTF::makeUnique<Task>(loader, updateBehavior, referrerPolicy);
  }
