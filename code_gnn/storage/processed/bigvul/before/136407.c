  void Update(const FloatRoundedRect& rrect,
              scoped_refptr<const RefCountedPath> path) {
    IntRect layer_bounds = EnclosingIntRect(rrect.Rect());
    gfx::Vector2dF new_layer_origin(layer_bounds.X(), layer_bounds.Y());

    SkRRect new_local_rrect = rrect;
    new_local_rrect.offset(-new_layer_origin.x(), -new_layer_origin.y());

    bool path_in_layer_changed = false;
    if (path_ == path) {
      path_in_layer_changed = path && layer_origin_ != new_layer_origin;
    } else if (!path_ || !path) {
      path_in_layer_changed = true;
    } else {
      SkPath new_path = path->GetSkPath();
      new_path.offset(layer_origin_.x() - new_layer_origin.x(),
                      layer_origin_.y() - new_layer_origin.y());
      path_in_layer_changed = path_->GetSkPath() != new_path;
    }

    if (local_rrect_ != new_local_rrect || path_in_layer_changed) {
      layer_->SetNeedsDisplay();
    }
    layer_->set_offset_to_transform_parent(new_layer_origin);
    layer_->SetBounds(gfx::Size(layer_bounds.Width(), layer_bounds.Height()));

    layer_origin_ = new_layer_origin;
    local_rrect_ = new_local_rrect;
    path_ = std::move(path);
  }
