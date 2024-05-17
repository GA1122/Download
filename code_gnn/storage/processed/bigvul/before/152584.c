FrameReplicationState ReconstructReplicationStateForTesting(
    TestRenderFrame* test_render_frame) {
  blink::WebLocalFrame* frame = test_render_frame->GetWebFrame();

  FrameReplicationState result;
  result.name = frame->AssignedName().Utf8();
  result.unique_name = test_render_frame->unique_name();
  result.frame_policy.sandbox_flags = frame->EffectiveSandboxFlagsForTesting();
  result.origin = frame->GetSecurityOrigin();

  return result;
}
