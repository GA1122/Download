base::Value* CreateChoiceData(
    const Experiment& experiment,
    const std::set<std::string>& enabled_experiments) {
  DCHECK(experiment.type == Experiment::MULTI_VALUE ||
         experiment.type == Experiment::ENABLE_DISABLE_VALUE);
  base::ListValue* result = new base::ListValue;
  for (int i = 0; i < experiment.num_choices; ++i) {
    base::DictionaryValue* value = new base::DictionaryValue;
    const std::string name = experiment.NameForChoice(i);
    value->SetString("internal_name", name);
    value->SetString("description", experiment.DescriptionForChoice(i));
    value->SetBoolean("selected", enabled_experiments.count(name) > 0);
    result->Append(value);
  }
  return result;
}
