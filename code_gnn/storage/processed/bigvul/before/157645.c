testing::AssertionResult ExtensionTabsZoomTest::RunGetZoom(
    int tab_id,
    double* zoom_factor) {
  scoped_refptr<TabsGetZoomFunction> get_zoom_function(
      new TabsGetZoomFunction());
  get_zoom_function->set_extension(extension_.get());
  get_zoom_function->set_has_callback(true);

  std::unique_ptr<base::Value> get_zoom_result(
      utils::RunFunctionAndReturnSingleResult(
          get_zoom_function.get(), base::StringPrintf("[%u]", tab_id),
          browser()));

  if (!get_zoom_result)
    return testing::AssertionFailure() << "no result";
  if (!get_zoom_result->GetAsDouble(zoom_factor))
    return testing::AssertionFailure() << "result was not a double";

  return testing::AssertionSuccess();
}
