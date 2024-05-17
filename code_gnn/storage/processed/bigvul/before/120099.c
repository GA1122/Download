void Layer::AddDependentNeedsPushProperties() {
  DCHECK_GE(num_dependents_need_push_properties_, 0);

  if (!parent_should_know_need_push_properties() && parent_)
    parent_->AddDependentNeedsPushProperties();

  num_dependents_need_push_properties_++;
}
