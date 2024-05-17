void AddInternalName(const Experiment& e, std::set<std::string>* names) {
  if (e.type == Experiment::SINGLE_VALUE) {
    names->insert(e.internal_name);
  } else {
    DCHECK(e.type == Experiment::MULTI_VALUE ||
           e.type == Experiment::ENABLE_DISABLE_VALUE);
    for (int i = 0; i < e.num_choices; ++i)
      names->insert(e.NameForChoice(i));
  }
}
