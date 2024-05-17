float GM2TabStyle::GetTopCornerRadiusForWidth(int width) {
  const int ideal_radius = GetCornerRadius();
  const int top_width = width - ideal_radius * 2;

  const float radius = top_width / 3.f;
  return base::ClampToRange<float>(radius, 0, ideal_radius);
}
