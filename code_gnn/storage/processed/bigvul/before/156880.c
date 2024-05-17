SandboxFlags DocumentInit::GetSandboxFlags() const {
  DCHECK(MasterDocumentLoader());
  DocumentLoader* loader = MasterDocumentLoader();
  SandboxFlags flags = loader->GetFrame()->Loader().EffectiveSandboxFlags();

  if (loader->WasBlockedAfterCSP()) {
    flags |= kSandboxOrigin;
  }

  return flags;
}
