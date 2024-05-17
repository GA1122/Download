void RenderViewHostManager::RendererProcessClosing(
    RenderProcessHost* render_process_host) {
  std::list<int> ids_to_remove;
  for (RenderViewHostMap::iterator iter = swapped_out_hosts_.begin();
       iter != swapped_out_hosts_.end();
       ++iter) {
    if (iter->second->GetProcess() == render_process_host)
      ids_to_remove.push_back(iter->first);
  }

  while (!ids_to_remove.empty()) {
    swapped_out_hosts_[ids_to_remove.back()]->Shutdown();
    swapped_out_hosts_.erase(ids_to_remove.back());
    ids_to_remove.pop_back();
  }
}
