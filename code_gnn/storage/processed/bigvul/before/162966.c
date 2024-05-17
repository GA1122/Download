 ManifestManager::ManifestManager(RenderFrame* render_frame)
     : RenderFrameObserver(render_frame),
       may_have_manifest_(false),
      manifest_dirty_(true) {}
