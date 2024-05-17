void TaskManagerHandler::HandleInspect(const ListValue* unique_ids) {
  for (ListValue::const_iterator i = unique_ids->begin();
       i != unique_ids->end(); ++i) {
    int unique_id = parseIndex(*i);
    int resource_index = model_->GetResourceIndexByUniqueId(unique_id);
    if (resource_index == -1)
      continue;

    if (model_->CanInspect(resource_index))
      model_->Inspect(resource_index);
    break;
  }
}
