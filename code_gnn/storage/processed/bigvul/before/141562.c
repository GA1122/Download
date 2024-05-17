void TracingControllerImpl::OnMetadataAvailable(base::Value metadata) {
  DCHECK(!filtered_metadata_);
  is_metadata_available_ = true;
  MetadataFilterPredicate metadata_filter;
  if (trace_config_->IsArgumentFilterEnabled()) {
    if (delegate_)
      metadata_filter = delegate_->GetMetadataFilterPredicate();
  }
  if (metadata_filter.is_null()) {
    filtered_metadata_ = base::DictionaryValue::From(
        base::Value::ToUniquePtrValue(std::move(metadata)));
  } else {
    filtered_metadata_ = std::make_unique<base::DictionaryValue>();
    for (auto it : metadata.DictItems()) {
      if (metadata_filter.Run(it.first)) {
        filtered_metadata_->SetKey(it.first, std::move(it.second));
      } else {
        filtered_metadata_->SetKey(it.first, base::Value("__stripped__"));
      }
    }
  }
  if (is_data_complete_)
    CompleteFlush();
}
