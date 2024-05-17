std::string SkColorToRGBComponents(SkColor color) {
  return base::StringPrintf(
      "%d,%d,%d",
      SkColorGetR(color),
      SkColorGetG(color),
      SkColorGetB(color));
}
