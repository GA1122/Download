 void AddResourceIcon(const gfx::ImageSkia* skia_image, void* data) {
   views::View* parent = static_cast<views::View*>(data);
   views::ImageView* image_view = new views::ImageView();
  image_view->SetImage(*skia_image);
  parent->AddChildView(image_view);
}
