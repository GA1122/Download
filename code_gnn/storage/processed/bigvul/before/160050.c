void BackendImpl::IncreaseNumRefs() {
  num_refs_++;
  if (max_refs_ < num_refs_)
    max_refs_ = num_refs_;
}
