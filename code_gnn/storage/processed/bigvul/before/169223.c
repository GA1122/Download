bool ExecuteScriptWithUserGestureControl(RenderFrameHost* frame,
                                         const std::string& script,
                                         bool user_gesture) {

  std::string expected_response = "ExecuteScript-" + base::GenerateGUID();
  std::string new_script = base::StringPrintf(
      R"( %s;  // Original script.
          window.domAutomationController.send('%s'); )",
      script.c_str(), expected_response.c_str());

  std::unique_ptr<base::Value> value;
  if (!ExecuteScriptHelper(frame, new_script, user_gesture, &value) ||
      !value.get()) {
    return false;
  }

  DCHECK_EQ(base::Value::Type::STRING, value->type());
  std::string actual_response;
  if (value->GetAsString(&actual_response))
    DCHECK_EQ(expected_response, actual_response);

  return true;
}
