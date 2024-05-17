std::unique_ptr<corewm::Tooltip> DesktopWindowTreeHostX11::CreateTooltip() {
  return base::WrapUnique(new corewm::TooltipAura);
}
