static RenderPass* FindRenderPassById(const RenderPassList& list, int id) {
  auto it = std::find_if(
      list.begin(), list.end(),
      [id](const std::unique_ptr<RenderPass>& p) { return p->id == id; });
  return it == list.end() ? nullptr : it->get();
}
