void CopyFeatureSwitch(const base::CommandLine& src,
                       base::CommandLine* dest,
                       const char* switch_name) {
  std::vector<std::string> features = FeaturesFromSwitch(src, switch_name);
  if (!features.empty())
    dest->AppendSwitchASCII(switch_name, base::JoinString(features, ","));
}
