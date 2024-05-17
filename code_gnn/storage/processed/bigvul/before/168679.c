void UiSceneCreator::CreateController() {
  auto root = base::MakeUnique<UiElement>();
  root->SetName(kControllerRoot);
  root->set_hit_testable(false);
  root->AddBinding(VR_BIND_FUNC(
      bool, Model, model_,
      browsing_mode() || model->web_vr_timeout_state == kWebVrTimedOut,
      UiElement, root.get(), SetVisible));
  scene_->AddUiElement(kRoot, std::move(root));

  auto group = base::MakeUnique<UiElement>();
  group->SetName(kControllerGroup);
  group->set_hit_testable(false);
  group->SetTransitionedProperties({OPACITY});
  group->AddBinding(base::MakeUnique<Binding<bool>>(
      base::Bind(
          [](Model* m) {
            return !m->controller.quiescent || !m->skips_redraw_when_not_dirty;
          },
          base::Unretained(model_)),
      base::Bind(
          [](UiElement* e, const bool& visible) {
            e->SetTransitionDuration(base::TimeDelta::FromMilliseconds(
                visible ? kControllerFadeInMs : kControllerFadeOutMs));
            e->SetVisible(visible);
          },
          base::Unretained(group.get()))));
  scene_->AddUiElement(kControllerRoot, std::move(group));

  auto controller = base::MakeUnique<Controller>();
  controller->SetDrawPhase(kPhaseForeground);
  controller->AddBinding(VR_BIND_FUNC(gfx::Transform, Model, model_,
                                      controller.transform, Controller,
                                      controller.get(), set_local_transform));
  controller->AddBinding(
      VR_BIND_FUNC(bool, Model, model_,
                   controller.touchpad_button_state == UiInputManager::DOWN,
                   Controller, controller.get(), set_touchpad_button_pressed));
  controller->AddBinding(VR_BIND_FUNC(
      bool, Model, model_, controller.app_button_state == UiInputManager::DOWN,
      Controller, controller.get(), set_app_button_pressed));
  controller->AddBinding(VR_BIND_FUNC(
      bool, Model, model_, controller.home_button_state == UiInputManager::DOWN,
      Controller, controller.get(), set_home_button_pressed));
  controller->AddBinding(VR_BIND_FUNC(float, Model, model_, controller.opacity,
                                      Controller, controller.get(),
                                      SetOpacity));
  scene_->AddUiElement(kControllerGroup, std::move(controller));

  auto laser = base::MakeUnique<Laser>(model_);
  laser->SetDrawPhase(kPhaseForeground);
  laser->AddBinding(VR_BIND_FUNC(float, Model, model_, controller.opacity,
                                 Laser, laser.get(), SetOpacity));
  scene_->AddUiElement(kControllerGroup, std::move(laser));

  auto reticle = base::MakeUnique<Reticle>(scene_, model_);
  reticle->SetDrawPhase(kPhaseForeground);
  scene_->AddUiElement(kControllerGroup, std::move(reticle));
}
