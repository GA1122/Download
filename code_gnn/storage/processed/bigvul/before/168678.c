void UiSceneCreator::CreateContentQuad() {
  auto hit_plane = base::MakeUnique<InvisibleHitTarget>();
  hit_plane->SetName(kBackplane);
  hit_plane->SetDrawPhase(kPhaseForeground);
  hit_plane->SetSize(kBackplaneSize, kSceneHeight);
  scene_->AddUiElement(k2dBrowsingContentGroup, std::move(hit_plane));

  auto main_content = base::MakeUnique<ContentElement>(
      content_input_delegate_,
      base::Bind(&UiBrowserInterface::OnContentScreenBoundsChanged,
                 base::Unretained(browser_)));
  main_content->SetName(kContentQuad);
  main_content->SetDrawPhase(kPhaseForeground);
  main_content->SetSize(kContentWidth, kContentHeight);
  main_content->set_corner_radius(kContentCornerRadius);
  main_content->SetTransitionedProperties({BOUNDS});
  main_content->AddBinding(
      VR_BIND(bool, Model, model_, fullscreen, UiElement, main_content.get(),
              SetSize(value ? kFullscreenWidth : kContentWidth,
                      value ? kFullscreenHeight : kContentHeight)));
  main_content->AddBinding(
      VR_BIND_FUNC(gfx::Transform, Model, model_, projection_matrix,
                   ContentElement, main_content.get(), SetProjectionMatrix));
  main_content->AddBinding(VR_BIND_FUNC(unsigned int, Model, model_,
                                        content_texture_id, ContentElement,
                                        main_content.get(), SetTextureId));
  main_content->AddBinding(VR_BIND_FUNC(
      UiElementRenderer::TextureLocation, Model, model_, content_location,
      ContentElement, main_content.get(), SetTextureLocation));
  scene_->AddUiElement(k2dBrowsingContentGroup, std::move(main_content));

  scene_->set_background_distance(kContentDistance *
                                  kBackgroundDistanceMultiplier);
}
