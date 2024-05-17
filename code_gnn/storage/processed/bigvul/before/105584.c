void Automation::GetTabIds(std::vector<int>* tab_ids,
                           Error** error) {
  std::string error_msg;
  if (!SendGetTabIdsJSONRequest(automation(), tab_ids, &error_msg))
    *error = new Error(kUnknownError, error_msg);
}
