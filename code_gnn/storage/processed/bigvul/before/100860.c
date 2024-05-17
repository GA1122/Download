int ClearBrowsingDataFunction::GetRemovalMask() const {
  base::DictionaryValue* data_to_remove;
  if (args_->GetDictionary(1, &data_to_remove))
    return ParseRemovalMask(data_to_remove);
  else
    return 0;
}
