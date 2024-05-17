void ExtensionWindowLastFocusedTest::ActivateBrowserWindow(Browser* browser) {
  BrowserView* view = BrowserView::GetBrowserViewForBrowser(browser);
  EXPECT_NE(nullptr, view);
  views::Widget* widget = view->frame();
  EXPECT_NE(nullptr, widget);
  WidgetActivatedWaiter waiter(widget);
  waiter.ActivateAndWait();
}
