std::string ContextualSearchFieldTrial::GetParam(const std::string& name) {
  return variations::GetVariationParamValue(kContextualSearchFieldTrialName,
                                            name);
}
