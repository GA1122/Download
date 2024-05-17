ColorProfile GetShelfColorProfile() {
  const std::string switch_value =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kAshShelfColorScheme);

  ColorProfile color_profile(LumaRange::DARK, SaturationRange::MUTED);

  if (switch_value.find("light") != std::string::npos)
    color_profile.luma = LumaRange::LIGHT;
  else if (switch_value.find("normal") != std::string::npos)
    color_profile.luma = LumaRange::NORMAL;
  else if (switch_value.find("dark") != std::string::npos)
    color_profile.luma = LumaRange::DARK;

  if (switch_value.find("vibrant") != std::string::npos)
    color_profile.saturation = SaturationRange::VIBRANT;
  else if (switch_value.find("muted") != std::string::npos)
    color_profile.saturation = SaturationRange::MUTED;

  return color_profile;
}
