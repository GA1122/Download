SandboxFlags FrameLoader::EffectiveSandboxFlags() const {
  SandboxFlags flags = forced_sandbox_flags_;
  if (FrameOwner* frame_owner = frame_->Owner())
    flags |= frame_owner->GetSandboxFlags();
  if (Frame* parent_frame = frame_->Tree().Parent())
    flags |= parent_frame->GetSecurityContext()->GetSandboxFlags();
  return flags;
}
