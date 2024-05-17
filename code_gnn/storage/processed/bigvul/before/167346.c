  void GetMockInfo(
      int* remove_calls_out,
      int* last_data_type_mask_out,
      base::RepeatingCallback<bool(const GURL&)>* last_origin_filter_out) {
    DCHECK_NE(nullptr, service_.get());

    *remove_calls_out = service_->remove_calls();
    *last_data_type_mask_out = service_->last_data_type_mask();
    *last_origin_filter_out = service_->last_origin_filter();
  }
