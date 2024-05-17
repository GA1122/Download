bool ExtensionTabsZoomTest::RunSetZoom(int tab_id, double zoom_factor) {
  scoped_refptr<TabsSetZoomFunction> set_zoom_function(
      new TabsSetZoomFunction());
  set_zoom_function->set_extension(extension_.get());
  set_zoom_function->set_has_callback(true);

  return utils::RunFunction(
      set_zoom_function.get(),
      base::StringPrintf("[%u, %lf]", tab_id, zoom_factor), browser(),
      api_test_utils::NONE);
}
