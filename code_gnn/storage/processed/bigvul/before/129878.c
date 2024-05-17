scoped_ptr<BackgroundTracingConfig> CreateReactiveConfig() {
  base::DictionaryValue dict;

  dict.SetString("mode", "REACTIVE_TRACING_MODE");

  scoped_ptr<base::ListValue> rules_list(new base::ListValue());
  {
    scoped_ptr<base::DictionaryValue> rules_dict(new base::DictionaryValue());
    rules_dict->SetString("rule", "TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL");
    rules_dict->SetString("trigger_name", "reactive_test");
    rules_dict->SetString("category", "BENCHMARK");
    rules_list->Append(rules_dict.Pass());
  }
  dict.Set("configs", rules_list.Pass());

  scoped_ptr<BackgroundTracingConfig> config(
      BackgroundTracingConfigImpl::FromDict(&dict));

  EXPECT_TRUE(config);
  return config.Pass();
}
