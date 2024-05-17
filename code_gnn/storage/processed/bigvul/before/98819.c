static bool SilverlightColorIsTransparent(const std::string& color) {
  if (StartsWithASCII(color, "#", false)) {
    if ((color.length() == 5 && !StartsWithASCII(color, "#F", false)) ||
        (color.length() == 9 && !StartsWithASCII(color, "#FF", false)))
      return true;
  } else if (StartsWithASCII(color, "sc#", false)) {
    if (color.length() < 4)
      return false;
    std::string value_string = color.substr(3, std::string::npos);
    std::vector<std::string> components;
    SplitString(value_string, ',', &components);
    if (components.size() == 4 && !StartsWithASCII(components[0], "1", false))
      return true;
  } else if (LowerCaseEqualsASCII(color, "transparent")) {
    return true;
  }
  return false;
}
