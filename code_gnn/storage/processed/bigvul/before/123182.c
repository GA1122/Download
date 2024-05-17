void GetScreenInfoForWindow(WebScreenInfo* results, aura::Window* window) {
  const gfx::Display display = window ?
      gfx::Screen::GetScreenFor(window)->GetDisplayNearestWindow(window) :
      gfx::Screen::GetScreenFor(window)->GetPrimaryDisplay();
  results->rect = display.bounds();
  results->availableRect = display.work_area();
  results->depth = 24;
  results->depthPerComponent = 8;
  results->deviceScaleFactor = display.device_scale_factor();
}
