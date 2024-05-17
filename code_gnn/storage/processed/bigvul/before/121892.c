  void CreatePrefList(const char* pref) {
    ListPrefUpdate update(&pref_service_, pref);
    update->Clear();
    for (size_t i = 0; i < kNumDaysInHistory; ++i) {
      update->Insert(0, new StringValue(base::Int64ToString(0)));
    }
  }
