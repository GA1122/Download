  static void Callback(TraceTicks timestamp,
                       char phase,
                       const unsigned char* category_group_enabled,
                       const char* name,
                       unsigned long long id,
                       int num_args,
                       const char* const arg_names[],
                       const unsigned char arg_types[],
                       const unsigned long long arg_values[],
                       unsigned int flags) {
    s_instance->collected_events_phases_.push_back(phase);
    s_instance->collected_events_categories_.push_back(
        TraceLog::GetCategoryGroupName(category_group_enabled));
    s_instance->collected_events_names_.push_back(name);
    s_instance->collected_events_timestamps_.push_back(timestamp);
  }
