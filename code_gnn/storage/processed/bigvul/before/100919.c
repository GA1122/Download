void TaskManagerHandler::OnModelChanged() {
  const int count = model_->GroupCount();

  base::FundamentalValue start_value(0);
  base::FundamentalValue length_value(count);
  base::ListValue tasks_value;
  for (int i = 0; i < count; ++i)
    tasks_value.Append(CreateTaskGroupValue(model_, i));

  if (is_enabled_) {
    web_ui_->CallJavascriptFunction("taskChanged",
                                    start_value, length_value, tasks_value);
  }
}
