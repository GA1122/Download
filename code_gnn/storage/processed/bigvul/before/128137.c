void CastDuplexView::Layout() {
  views::View::Layout();

  if (select_view_->IsDrawn())
    select_view_->SetBoundsRect(GetContentsBounds());
  if (cast_view_->IsDrawn())
    cast_view_->SetBoundsRect(GetContentsBounds());
}
