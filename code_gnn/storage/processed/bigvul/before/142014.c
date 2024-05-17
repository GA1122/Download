  explicit CallbackOwner(bool* deleted) {
    callback_ = BindRepeating(&CallbackOwner::Unused, WrapRefCounted(this));
    deleted_ = deleted;
  }
