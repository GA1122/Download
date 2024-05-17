bool GenerateInfoSpec(const std::string& values, int* result) {
  base::ListValue list_value;
  for (const std::string& cur :
       base::SplitString(values, ",", base::KEEP_WHITESPACE,
                         base::SPLIT_WANT_NONEMPTY))
    list_value.AppendString(cur);
  return ExtraInfoSpec::InitFromValue(list_value, result);
}
