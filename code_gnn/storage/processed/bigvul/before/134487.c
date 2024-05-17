  ui::Layer* CreateSlideLayer(int offset) {
    const NavigationControllerImpl& controller = web_contents_->GetController();
    const NavigationEntryImpl* entry = NavigationEntryImpl::FromNavigationEntry(
        controller.GetEntryAtOffset(offset));

    gfx::Image image;
    if (entry && entry->screenshot().get()) {
      std::vector<gfx::ImagePNGRep> image_reps;
      image_reps.push_back(gfx::ImagePNGRep(entry->screenshot(),
          ui::GetImageScale(
              ui::GetScaleFactorForNativeView(window_.get()))));
      image = gfx::Image(image_reps);
    }
    layer_delegate_.SetImage(image);

    ui::Layer* layer = new ui::Layer(ui::LAYER_TEXTURED);
    layer->set_delegate(&layer_delegate_);
    return layer;
  }
