int GetZoomPercent(const content::WebContents* contents,
                   bool* enable_plus,
                   bool* enable_minus) {
  int percent =
      zoom::ZoomController::FromWebContents(contents)->GetZoomPercent();
  *enable_plus = percent < contents->GetMaximumZoomPercent();
  *enable_minus = percent > contents->GetMinimumZoomPercent();
  return percent;
}
