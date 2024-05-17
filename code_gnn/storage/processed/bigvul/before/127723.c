bool HasWarning(const WarningSet& warnings,
                const std::string& extension_id) {
  for (WarningSet::const_iterator i = warnings.begin();
       i != warnings.end(); ++i) {
    if (i->extension_id() == extension_id)
      return true;
  }
  return false;
}
