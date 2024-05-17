bool InitExperiment(disk_cache::IndexHeader* header, bool cache_created) {
  if (header->experiment == disk_cache::EXPERIMENT_OLD_FILE1 ||
      header->experiment == disk_cache::EXPERIMENT_OLD_FILE2) {
    return false;
  }

  if (base::FieldTrialList::FindFullName("SimpleCacheTrial") ==
          "ExperimentControl") {
    if (cache_created) {
      header->experiment = disk_cache::EXPERIMENT_SIMPLE_CONTROL;
      return true;
    }
    return header->experiment == disk_cache::EXPERIMENT_SIMPLE_CONTROL;
  }

  header->experiment = disk_cache::NO_EXPERIMENT;
  return true;
}
