void AddInternalName(const Experiment& e, std::set<std::string>* names) {
  if (e.type == Experiment::SINGLE_VALUE) {
    names->insert(e.internal_name);
  } else {
    DCHECK_EQ(Experiment::MULTI_VALUE, e.type);
    for (int i = 0; i < e.num_choices; ++i)
      names->insert(NameForChoice(e, i));
  }
}
