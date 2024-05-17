void RemoveFlagsSwitches(
    std::map<std::string, base::CommandLine::StringType>* switch_list) {
  FlagsStateSingleton::GetFlagsState()->RemoveFlagsSwitches(switch_list);
}
