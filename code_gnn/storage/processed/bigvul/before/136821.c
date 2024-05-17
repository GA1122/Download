int LocalDOMWindow::orientation() const {
  DCHECK(RuntimeEnabledFeatures::OrientationEventEnabled());

  if (!GetFrame() || !GetFrame()->GetPage())
    return 0;

  int orientation = GetFrame()
                        ->GetPage()
                        ->GetChromeClient()
                        .GetScreenInfo()
                        .orientation_angle;
  if (orientation == 270)
    return -90;
  return orientation;
}
