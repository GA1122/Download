void ProcessInfoObserver::OnDetailsAvailable() {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  ListValue* browser_proc_list = new ListValue();
  const std::vector<ProcessData>& all_processes = processes();
  for (size_t index = 0; index < all_processes.size(); ++index) {
    DictionaryValue* browser_data = new DictionaryValue();
    browser_data->SetString("name", all_processes[index].name);
    browser_data->SetString("process_name", all_processes[index].process_name);

    ListValue* proc_list = new ListValue();
    for (ProcessMemoryInformationList::const_iterator iterator =
             all_processes[index].processes.begin();
         iterator != all_processes[index].processes.end(); ++iterator) {
      DictionaryValue* proc_data = new DictionaryValue();

      proc_data->SetInteger("pid", iterator->pid);

      DictionaryValue* working_set = new DictionaryValue();
      working_set->SetInteger("priv", iterator->working_set.priv);
      working_set->SetInteger("shareable", iterator->working_set.shareable);
      working_set->SetInteger("shared", iterator->working_set.shared);
      proc_data->Set("working_set_mem", working_set);

      DictionaryValue* committed = new DictionaryValue();
      committed->SetInteger("priv", iterator->committed.priv);
      committed->SetInteger("mapped", iterator->committed.mapped);
      committed->SetInteger("image", iterator->committed.image);
      proc_data->Set("committed_mem", committed);

      proc_data->SetString("version", iterator->version);
      proc_data->SetString("product_name", iterator->product_name);
      proc_data->SetInteger("num_processes", iterator->num_processes);
      proc_data->SetBoolean("is_diagnostics", iterator->is_diagnostics);

      std::string process_type = "Unknown";
      if (iterator->type != content::PROCESS_TYPE_UNKNOWN)
        process_type = content::GetProcessTypeNameInEnglish(iterator->type);
      proc_data->SetString("child_process_type", process_type);

      std::string renderer_type = "Unknown";
      if (iterator->renderer_type !=
          ProcessMemoryInformation::RENDERER_UNKNOWN) {
        renderer_type = ProcessMemoryInformation::GetRendererTypeNameInEnglish(
            iterator->renderer_type);
      }
      proc_data->SetString("renderer_type", renderer_type);

      ListValue* titles = new ListValue();
      for (size_t title_index = 0; title_index < iterator->titles.size();
           ++title_index)
        titles->Append(Value::CreateStringValue(iterator->titles[title_index]));
      proc_data->Set("titles", titles);

      proc_list->Append(proc_data);
    }
    browser_data->Set("processes", proc_list);

    browser_proc_list->Append(browser_data);
  }
  return_value->Set("browsers", browser_proc_list);

  if (automation_) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(return_value.get());
  }
}
