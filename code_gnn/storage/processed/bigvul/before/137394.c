std::unique_ptr<ResizeParams> RenderViewTest::InitialSizeParams() {
  auto initial_size = std::make_unique<ResizeParams>();
  initial_size->new_size = gfx::Size(400, 300);
  initial_size->visible_viewport_size = gfx::Size(400, 300);
  return initial_size;
}
