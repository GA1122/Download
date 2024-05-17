void Automation::ExecuteScript(int tab_id,
                               const FramePath& frame_path,
                               const std::string& script,
                               std::string* result,
                               Error** error) {
  int windex = 0, tab_index = 0;
  *error = GetIndicesForTab(tab_id, &windex, &tab_index);
  if (*error)
    return;

  Value* unscoped_value;
  std::string error_msg;
  if (!SendExecuteJavascriptJSONRequest(automation(), windex, tab_index,
                                        frame_path.value(), script,
                                        &unscoped_value, &error_msg)) {
    *error = new Error(kUnknownError, error_msg);
    return;
  }
  scoped_ptr<Value> value(unscoped_value);
  if (!value->GetAsString(result))
    *error = new Error(kUnknownError, "Execute script did not return string");
}
