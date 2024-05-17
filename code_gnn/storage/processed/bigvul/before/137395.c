void RenderViewTest::RendererBlinkPlatformImplTestOverride::Initialize() {
  renderer_scheduler_ = blink::scheduler::RendererScheduler::Create();
  blink_platform_impl_ =
      std::make_unique<RendererBlinkPlatformImplTestOverrideImpl>(
          renderer_scheduler_.get());
}
