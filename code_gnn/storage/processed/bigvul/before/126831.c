  virtual void OnPaint(gfx::Canvas* canvas) {
    views::Widget* widget = GetWidget();
    if (!widget || (widget->IsMaximized() || widget->IsFullscreen()))
      return;

    gfx::ImageSkia* image = ui::ResourceBundle::GetSharedInstance().
        GetImageSkiaNamed(IDR_TEXTAREA_RESIZER);
    canvas->DrawImageInt(*image, width() - image->width(),
                         height() - image->height());
  }
