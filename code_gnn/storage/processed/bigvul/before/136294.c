static bool IsRepeatingTableSection(const LayoutObject& object) {
  if (!object.IsTableSection())
    return false;
  const auto& section = ToLayoutTableSection(object);
  return section.IsRepeatingHeaderGroup() || section.IsRepeatingFooterGroup();
}
