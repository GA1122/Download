std::string Experiment::NameForChoice(int index) const {
  DCHECK(type == Experiment::MULTI_VALUE ||
         type == Experiment::ENABLE_DISABLE_VALUE);
  DCHECK_LT(index, num_choices);
  return std::string(internal_name) + testing::kMultiSeparator +
         base::IntToString(index);
}
