void HTMLFrameOwnerElement::SetSandboxFlags(SandboxFlags flags) {
  sandbox_flags_ = flags;
  container_policy_ = ConstructContainerPolicy(nullptr, nullptr);

  if (ContentFrame()) {
    GetDocument().GetFrame()->Client()->DidChangeFramePolicy(
        ContentFrame(), sandbox_flags_, container_policy_);
  }
}
