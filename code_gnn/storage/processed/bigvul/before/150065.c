void LayerTreeHostImpl::SynchronouslyInitializeAllTiles() {
  single_thread_synchronous_task_graph_runner_->RunUntilIdle();
}
