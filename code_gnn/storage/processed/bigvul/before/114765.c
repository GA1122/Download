  explicit CreateRootHelper(ProfileSyncServiceSessionTest* test)
      : ALLOW_THIS_IN_INITIALIZER_LIST(callback_(
            base::Bind(&CreateRootHelper::CreateRootCallback,
                       base::Unretained(this), test))),
        success_(false) {
  }
