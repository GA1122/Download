views::View* MetalayerMode::CreateView() {
  views::View* view = CreateDefaultView(base::string16());
  UpdateView();
  return view;
}
