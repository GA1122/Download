void TaskManagerHandler::OnGroupChanged(const int group_start,
                                        const int group_length) {
  base::FundamentalValue start_value(group_start);
  base::FundamentalValue length_value(group_length);
  base::ListValue tasks_value;

  for (int i = 0; i < group_length; ++i)
    tasks_value.Append(CreateTaskGroupValue(model_, group_start + i));

  if (is_enabled_ && is_alive()) {
    web_ui_->CallJavascriptFunction("taskChanged",
                                    start_value, length_value, tasks_value);
  }
}
