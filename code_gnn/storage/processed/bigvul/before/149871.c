bool LayerTreeHost::UpdateLayers() {
  if (!root_layer()) {
    property_trees_.clear();
    return false;
  }
  DCHECK(!root_layer()->parent());
  base::ElapsedTimer timer;

  bool result = DoUpdateLayers(root_layer());
  micro_benchmark_controller_.DidUpdateLayers();

  if (const char* client_name = GetClientNameForMetrics()) {
    std::string histogram_name =
        base::StringPrintf("Compositing.%s.LayersUpdateTime.%d", client_name,
                           GetLayersUpdateTimeHistogramBucket(NumLayers()));
    base::Histogram::FactoryGet(histogram_name, 0, 10000000, 50,
                                base::HistogramBase::kUmaTargetedHistogramFlag)
        ->Add(timer.Elapsed().InMicroseconds());
  }

  return result || next_commit_forces_redraw_;
}
