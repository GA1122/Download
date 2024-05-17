bool SkipConditionalExperiment(const Experiment& experiment) {
  if (experiment.internal_name ==
      std::string("enhanced-bookmarks-experiment")) {
    CommandLine* command_line = CommandLine::ForCurrentProcess();
    if (command_line->HasSwitch(switches::kEnhancedBookmarksExperiment))
      return false;

    return !IsEnhancedBookmarksExperimentEnabled();
  }
  if ((experiment.internal_name == std::string("manual-enhanced-bookmarks")) ||
      (experiment.internal_name ==
           std::string("manual-enhanced-bookmarks-optout"))) {
    return true;
  }

  return false;
}
