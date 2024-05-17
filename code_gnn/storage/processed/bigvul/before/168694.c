void UiSceneCreator::CreateWebVrRoot() {
  auto element = base::MakeUnique<UiElement>();
  element->SetName(kWebVrRoot);
  element->set_hit_testable(false);
  element->AddBinding(VR_BIND_FUNC(bool, Model, model_,
                                   browsing_mode() == false, UiElement,
                                   element.get(), SetVisible));
  scene_->AddUiElement(kRoot, std::move(element));
}
