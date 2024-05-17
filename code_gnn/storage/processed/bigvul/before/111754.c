Value* CreateChoiceData(const Experiment& experiment,
                        const std::set<std::string>& enabled_experiments) {
  DCHECK_EQ(Experiment::MULTI_VALUE, experiment.type);
  ListValue* result = new ListValue;
  for (int i = 0; i < experiment.num_choices; ++i) {
    const Experiment::Choice& choice = experiment.choices[i];
    DictionaryValue* value = new DictionaryValue;
    std::string name = NameForChoice(experiment, i);
    value->SetString("description",
                     l10n_util::GetStringUTF16(choice.description_id));
    value->SetString("internal_name", name);
    value->SetBoolean("selected", enabled_experiments.count(name) > 0);
    result->Append(value);
  }
  return result;
}
