  views::Widget* CreateTestWidget() {
    views::Widget::InitParams params(views::Widget::InitParams::TYPE_WINDOW);
    params.bounds = gfx::Rect(0, 0, 200, 200);
    params.context = CurrentContext();
    views::Widget* widget = new views::Widget;
    widget->Init(params);
    widget->Show();
    return widget;
  }
