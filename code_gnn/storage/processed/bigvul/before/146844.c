bool Document::ShouldScheduleLayout() const {
  if (!IsActive())
    return false;

  if (IsRenderingReady() && body())
    return true;

  if (documentElement() && !isHTMLHtmlElement(*documentElement()))
    return true;

  return false;
}
