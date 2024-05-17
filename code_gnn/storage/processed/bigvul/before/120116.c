bool Layer::IsContainerForFixedPositionLayers() const {
  if (!transform_.IsIdentityOrTranslation())
    return true;
  if (parent_ && !parent_->transform_.IsIdentityOrTranslation())
    return true;
  return is_container_for_fixed_position_layers_;
}
