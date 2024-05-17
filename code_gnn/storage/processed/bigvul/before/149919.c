ScrollElasticityHelper* LayerTreeHostImpl::CreateScrollElasticityHelper() {
  DCHECK(!scroll_elasticity_helper_);
  if (settings_.enable_elastic_overscroll) {
    scroll_elasticity_helper_.reset(
        ScrollElasticityHelper::CreateForLayerTreeHostImpl(this));
  }
  return scroll_elasticity_helper_.get();
}
