void RenderViewTest::GoBack(const GURL& url, const PageState& state) {
  GoToOffset(-1, url, state);
}
