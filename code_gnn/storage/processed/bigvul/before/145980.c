void CheckHasCaptureAndReleaseCapture(aura::Window* window) {
  ASSERT_TRUE(window->HasCapture());
  window->ReleaseCapture();
}
