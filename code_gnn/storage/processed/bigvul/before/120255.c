bool LayerTreeHost::ScheduleMicroBenchmark(
    const std::string& benchmark_name,
    scoped_ptr<base::Value> value,
    const MicroBenchmark::DoneCallback& callback) {
  return micro_benchmark_controller_.ScheduleRun(
      benchmark_name, value.Pass(), callback);
}
