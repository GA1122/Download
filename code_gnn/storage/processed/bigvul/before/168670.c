TransientElement* AddTransientParent(UiElementName name,
                                     UiElementName parent_name,
                                     int timeout_seconds,
                                     bool animate_opacity,
                                     UiScene* scene) {
  auto element = base::MakeUnique<SimpleTransientElement>(
      base::TimeDelta::FromSeconds(timeout_seconds));
  TransientElement* to_return = element.get();
  element->SetName(name);
  element->SetVisible(false);
  element->set_hit_testable(false);
  if (animate_opacity)
    element->SetTransitionedProperties({OPACITY});
  scene->AddUiElement(parent_name, std::move(element));
  return to_return;
}
