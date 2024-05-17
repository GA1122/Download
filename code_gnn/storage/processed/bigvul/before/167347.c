  void GetMockInfo(
      int* remove_calls_out,
      base::RepeatingCallback<bool(const GURL&)>* last_origin_filter_out) {
    DCHECK_NE(nullptr, delegate_.get());

    *remove_calls_out = delegate_->remove_calls();
    *last_origin_filter_out = delegate_->last_origin_filter();
  }
