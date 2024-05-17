void SetFlagToSwitchMapping(const std::string& key,
                            const std::string& switch_name,
                            const std::string& switch_value,
                            NameToSwitchAndValueMap* name_to_switch_map) {
  DCHECK(name_to_switch_map->end() == name_to_switch_map->find(key));
  (*name_to_switch_map)[key] = std::make_pair(switch_name, switch_value);
}
