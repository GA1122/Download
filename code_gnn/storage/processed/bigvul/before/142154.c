  bool CreateRootDirectory(const Profile* profile) {
    if (root_initialized_)
      return true;
    root_ = profile->GetPath().Append(name_);
    base::ScopedAllowBlockingForTesting allow_blocking;
    root_initialized_ = base::CreateDirectory(root_);
    return root_initialized_;
  }
