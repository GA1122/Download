void ContinueAndCompleteDrag(ui::test::EventGenerator* generator,
                             wm::WindowState* window_state,
                             aura::Window* window) {
  ASSERT_TRUE(window->HasCapture());
  ASSERT_FALSE(window_state->GetWindowPositionManaged());
  generator->DragMouseBy(100, 100);
  generator->ReleaseLeftButton();
}
