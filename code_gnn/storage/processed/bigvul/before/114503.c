RenderingHelper::~RenderingHelper() {
  CHECK_EQ(width_, 0) << "Must call UnInitialize before dtor.";
  Clear();
}
