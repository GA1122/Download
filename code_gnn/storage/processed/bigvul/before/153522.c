int GM2TabStyle::GetStrokeThickness(bool should_paint_as_active) const {
  return (tab_->IsActive() || should_paint_as_active)
             ? tab_->controller()->GetStrokeThickness()
              : 0;
 }
