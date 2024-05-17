scoped_ptr<BackgroundTracingConfig> CreatePreemptiveConfig() {
  base::DictionaryValue dict;

  dict.SetString("mode", "PREEMPTIVE_TRACING_MODE");
  dict.SetString("category", "BENCHMARK");

  scoped_ptr<base::ListValue> rules_list(new base::ListValue());
  {
    scoped_ptr<base::DictionaryValue> rules_dict(new base::DictionaryValue());
    rules_dict->SetString("rule", "MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED");
    rules_dict->SetString("trigger_name", "preemptive_test");
    rules_list->Append(rules_dict.Pass());
  }
  dict.Set("configs", rules_list.Pass());

  scoped_ptr<BackgroundTracingConfig> config(
      BackgroundTracingConfigImpl::FromDict(&dict));

  EXPECT_TRUE(config);
  return config.Pass();
}
