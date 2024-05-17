void InputType::CountUsageIfVisible(WebFeature feature) const {
  if (const ComputedStyle* style = GetElement().GetComputedStyle()) {
    if (style->Visibility() != EVisibility::kHidden)
      UseCounter::Count(GetElement().GetDocument(), feature);
  }
}
