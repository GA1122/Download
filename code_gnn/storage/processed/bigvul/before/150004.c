void LayerTreeHostImpl::RemoveRenderPasses(FrameData* frame) {
  DCHECK_GE(frame->render_passes.size(), 1u);

  std::set<int> pass_exists;
  base::SmallMap<std::unordered_map<int, int>> pass_references;

  for (size_t i = 0; i < frame->render_passes.size(); ++i) {
    RenderPass* pass = frame->render_passes[i].get();

    for (auto it = pass->quad_list.begin(); it != pass->quad_list.end();) {
      if (it->material != DrawQuad::RENDER_PASS) {
        ++it;
        continue;
      }
      const RenderPassDrawQuad* quad = RenderPassDrawQuad::MaterialCast(*it);
      if (pass_exists.count(quad->render_pass_id)) {
        pass_references[quad->render_pass_id]++;
        ++it;
      } else {
        it = pass->quad_list.EraseAndInvalidateAllPointers(it);
      }
    }

    if (i == frame->render_passes.size() - 1) {
      break;
    }

    if (pass->quad_list.empty() && pass->copy_requests.empty()) {
      frame->render_passes.erase(frame->render_passes.begin() + i);
      --i;
      continue;
    }

    pass_exists.insert(pass->id);
  }

  for (size_t i = 0; i < frame->render_passes.size() - 1; ++i) {
    RenderPass* pass =
        frame->render_passes[frame->render_passes.size() - 2 - i].get();
    if (!pass->copy_requests.empty())
      continue;
    if (pass_references[pass->id])
      continue;

    for (auto it = pass->quad_list.begin(); it != pass->quad_list.end(); ++it) {
      if (it->material != DrawQuad::RENDER_PASS)
        continue;
      const RenderPassDrawQuad* quad = RenderPassDrawQuad::MaterialCast(*it);
      pass_references[quad->render_pass_id]--;
    }

    frame->render_passes.erase(frame->render_passes.end() - 2 - i);
    --i;
  }
}
