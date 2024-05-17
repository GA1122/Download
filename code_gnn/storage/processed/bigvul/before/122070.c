std::string ProfileDependencyManager::DumpGraphvizDependency() {
  std::string result("digraph {\n");

  std::deque<ProfileKeyedBaseFactory*> components;
  std::copy(all_components_.begin(),
            all_components_.end(),
            std::back_inserter(components));

  std::deque<ProfileKeyedBaseFactory*>::iterator components_end =
      components.end();
  result.append("  /* Dependencies */\n");
  for (EdgeMap::const_iterator it = edges_.begin(); it != edges_.end(); ++it) {
    result.append("  ");
    result.append(it->second->name());
    result.append(" -> ");
    result.append(it->first->name());
    result.append(";\n");

    components_end = std::remove(components.begin(), components_end,
                                 it->second);
  }
  components.erase(components_end, components.end());

  result.append("\n  /* Toplevel attachments */\n");
  for (std::deque<ProfileKeyedBaseFactory*>::const_iterator it =
           components.begin(); it != components.end(); ++it) {
    result.append("  ");
    result.append((*it)->name());
    result.append(" -> Profile;\n");
  }

  result.append("\n  /* Toplevel profile */\n");
  result.append("  Profile [shape=box];\n");

  result.append("}\n");
  return result;
}
