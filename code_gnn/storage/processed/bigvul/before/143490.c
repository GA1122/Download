void CompositorImpl::DidFailToInitializeLayerTreeFrameSink() {
  layer_tree_frame_sink_request_pending_ = false;
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&CompositorImpl::RequestNewLayerTreeFrameSink,
                                weak_factory_.GetWeakPtr()));
}
