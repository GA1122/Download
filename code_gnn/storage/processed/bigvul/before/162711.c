static bool ValidateRectForCanvas(double& x,
                                  double& y,
                                  double& width,
                                  double& height) {
  if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) ||
      !std::isfinite(height))
    return false;

  if (!width && !height)
    return false;

  if (width < 0) {
    width = -width;
    x -= width;
  }

  if (height < 0) {
    height = -height;
    y -= height;
  }

  return true;
}
