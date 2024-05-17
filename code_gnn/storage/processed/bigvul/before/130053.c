void VariationsService::NotifyObservers(
    const variations::VariationsSeedSimulator::Result& result) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (result.kill_critical_group_change_count > 0) {
    FOR_EACH_OBSERVER(Observer, observer_list_,
                      OnExperimentChangesDetected(Observer::CRITICAL));
  } else if (result.kill_best_effort_group_change_count > 0) {
    FOR_EACH_OBSERVER(Observer, observer_list_,
                      OnExperimentChangesDetected(Observer::BEST_EFFORT));
  }
}
