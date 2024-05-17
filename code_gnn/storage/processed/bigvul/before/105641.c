HIMAGELIST TreeView::CreateImageList() {
  std::vector<SkBitmap> model_images;
  model_->GetIcons(&model_images);

  bool rtl = base::i18n::IsRTL();
  SkBitmap* closed_icon =
      ResourceBundle::GetSharedInstance().GetBitmapNamed(
          (rtl ? IDR_FOLDER_CLOSED_RTL : IDR_FOLDER_CLOSED));
  SkBitmap* opened_icon =
      ResourceBundle::GetSharedInstance().GetBitmapNamed(
          (rtl ? IDR_FOLDER_OPEN_RTL : IDR_FOLDER_OPEN));
  int width = closed_icon->width();
  int height = closed_icon->height();
  DCHECK(opened_icon->width() == width && opened_icon->height() == height);
  HIMAGELIST image_list =
      ImageList_Create(width, height, ILC_COLOR32, model_images.size() + 2,
                       model_images.size() + 2);
  if (image_list) {
    HICON h_closed_icon = IconUtil::CreateHICONFromSkBitmap(*closed_icon);
    HICON h_opened_icon = IconUtil::CreateHICONFromSkBitmap(*opened_icon);
    ImageList_AddIcon(image_list, h_closed_icon);
    ImageList_AddIcon(image_list, h_opened_icon);
    DestroyIcon(h_closed_icon);
    DestroyIcon(h_opened_icon);
    for (size_t i = 0; i < model_images.size(); ++i) {
      HICON model_icon;

      if (model_images[i].width() != width ||
          model_images[i].height() != height) {
        gfx::CanvasSkia canvas(width, height, false);
        canvas.drawColor(SK_ColorBLACK, SkXfermode::kClear_Mode);

        int height_offset = (height - model_images[i].height()) / 2;
        int width_offset = (width - model_images[i].width()) / 2;
        canvas.DrawBitmapInt(model_images[i], width_offset, height_offset);
        model_icon = IconUtil::CreateHICONFromSkBitmap(canvas.ExtractBitmap());
      } else {
        model_icon = IconUtil::CreateHICONFromSkBitmap(model_images[i]);
      }
      ImageList_AddIcon(image_list, model_icon);
      DestroyIcon(model_icon);
    }
  }
  return image_list;
}
