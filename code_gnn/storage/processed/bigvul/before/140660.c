void OutOfProcessInstance::SearchString(const base::char16* string,
                            const base::char16* term,
                            bool case_sensitive,
                            std::vector<SearchStringResult>* results) {
  PP_PrivateFindResult* pp_results;
  int count = 0;
  pp::PDF::SearchString(
      this,
      reinterpret_cast<const unsigned short*>(string),
      reinterpret_cast<const unsigned short*>(term),
      case_sensitive,
      &pp_results,
      &count);

  results->resize(count);
  for (int i = 0; i < count; ++i) {
    (*results)[i].start_index = pp_results[i].start_index;
    (*results)[i].length = pp_results[i].length;
  }

  pp::Memory_Dev memory;
  memory.MemFree(pp_results);
}
