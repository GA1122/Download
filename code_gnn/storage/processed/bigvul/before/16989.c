QStringList WebContext::hostMappingRules() const {
  std::vector<std::string> v;
  if (!IsInitialized()) {
    v = construct_props_->host_mapping_rules;
  } else {
    v = context_->GetHostMappingRules();
  }

  QStringList rules;
  for (std::vector<std::string>::const_iterator it = v.cbegin();
       it != v.cend(); ++it) {
    rules.append(QString::fromStdString(*it));
  }

  return rules;
}
