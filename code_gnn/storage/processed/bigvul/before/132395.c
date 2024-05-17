void RemoveFlagsSwitches(
    std::map<std::string, base::CommandLine::StringType>* switch_list) {
  FlagsState::GetInstance()->RemoveFlagsSwitches(switch_list);
}
