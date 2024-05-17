void ExtensionInfoBar::OnImageLoaded(
    SkBitmap* image, ExtensionResource resource, int index) {
  if (!delegate_)
    return;   

  ResourceBundle& rb = ResourceBundle::GetSharedInstance();

  SkBitmap* icon;
  if (!image || image->empty())
    icon = rb.GetBitmapNamed(IDR_EXTENSIONS_SECTION);
  else
    icon = image;

  SkBitmap* drop_image = rb.GetBitmapNamed(IDR_APP_DROPARROW);

  int image_size = Extension::EXTENSION_ICON_BITTY;
  scoped_ptr<gfx::CanvasSkia> canvas(
      new gfx::CanvasSkia(
          image_size + kDropArrowLeftMargin + drop_image->width(),
          image_size, false));
  canvas->DrawBitmapInt(*icon,
                        0, 0, icon->width(), icon->height(),
                        0, 0, image_size, image_size,
                        false);
  canvas->DrawBitmapInt(*drop_image,
                        image_size + kDropArrowLeftMargin,
                        image_size / 2);
  menu_->SetIcon(canvas->ExtractBitmap());
  menu_->SetVisible(true);

  Layout();
}
