std::string NameForChoice(const Experiment& e, int index) {
  DCHECK_EQ(Experiment::MULTI_VALUE, e.type);
  DCHECK_LT(index, e.num_choices);
  return std::string(e.internal_name) + about_flags::testing::kMultiSeparator +
      base::IntToString(index);
}
