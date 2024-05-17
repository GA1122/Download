bool Document::ShouldScheduleLayout() const {
  if (!IsActive())
    return false;

  if (IsRenderingReady() && body())
    return true;

  if (documentElement() && !IsHTMLHtmlElement(*documentElement()))
    return true;

  return false;
}
