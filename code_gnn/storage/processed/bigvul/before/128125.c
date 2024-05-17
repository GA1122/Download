CastDuplexView::CastDuplexView(SystemTrayItem* owner,
                               CastConfigDelegate* config_delegate,
                               bool show_more) {
  select_view_ = new CastSelectDefaultView(owner, config_delegate, show_more);
  cast_view_ = new CastCastView(config_delegate);
  SetLayoutManager(new views::FillLayout());
  AddChildView(select_view_);
  AddChildView(cast_view_);

  ActivateSelectView();
}
