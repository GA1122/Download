  void CreateFieldTrialWithParams(
      const char* trial_name,
      const char* group_name,
      const char* key1, const char* value1,
      const char* key2, const char* value2) {
    std::map<std::string, std::string> params;
    params.insert(std::make_pair(key1, value1));
    params.insert(std::make_pair(key2, value2));
    CreateFieldTrial(trial_name, kFakeGroupName);
    variations::AssociateVariationParams(trial_name, kFakeGroupName, params);
  }
