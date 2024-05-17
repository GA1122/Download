TabStrip::DropInfo::DropInfo(int drop_index,
                             bool drop_before,
                             bool point_down,
                             views::Widget* context)
    : drop_index(drop_index),
      drop_before(drop_before),
      point_down(point_down),
      file_supported(true) {
  arrow_view = new views::ImageView;
  arrow_view->SetImage(GetDropArrowImage(point_down));

  arrow_window = new views::Widget;
  views::Widget::InitParams params(views::Widget::InitParams::TYPE_POPUP);
  params.keep_on_top = true;
  params.opacity = views::Widget::InitParams::TRANSLUCENT_WINDOW;
  params.accept_events = false;
  params.can_activate = false;
  params.bounds = gfx::Rect(drop_indicator_width, drop_indicator_height);
  params.context = context->GetNativeView();
  arrow_window->Init(params);
  arrow_window->SetContentsView(arrow_view);
}
