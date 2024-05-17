  OverscrollWindowDelegate(WebContentsImpl* web_contents,
                           OverscrollMode overscroll_mode)
      : web_contents_(web_contents),
        forward_events_(true) {
    const NavigationControllerImpl& controller = web_contents->GetController();
    const NavigationEntryImpl* entry = NULL;
    if (ShouldNavigateForward(controller, overscroll_mode)) {
      entry = NavigationEntryImpl::FromNavigationEntry(
          controller.GetEntryAtOffset(1));
    } else if (ShouldNavigateBack(controller, overscroll_mode)) {
      entry = NavigationEntryImpl::FromNavigationEntry(
          controller.GetEntryAtOffset(-1));
    }

    gfx::Image image;
    if (entry && entry->screenshot().get()) {
      std::vector<gfx::ImagePNGRep> image_reps;
      image_reps.push_back(gfx::ImagePNGRep(entry->screenshot(),
          ui::GetImageScale(
              ui::GetScaleFactorForNativeView(web_contents_window()))));
      image = gfx::Image(image_reps);
    }
    SetImage(image);
  }
