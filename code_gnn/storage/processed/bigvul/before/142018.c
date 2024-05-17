  static bool IsCancelled(const internal::BindStateBase*,
                          internal::BindStateBase::CancellationQueryMode mode) {
    switch (mode) {
      case internal::BindStateBase::IS_CANCELLED:
        return false;
      case internal::BindStateBase::MAYBE_VALID:
        return true;
    }
    NOTREACHED();
  }
