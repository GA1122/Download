base::string16 Tab::GetTooltipText(const gfx::Point& p) const {
  if (base::FeatureList::IsEnabled(features::kTabHoverCards))
    return base::string16();

  return GetTooltipText(data_.title, data_.alert_state);
}
