    sandbox::bpf_dsl::Policy* (*broker_sandboxer_allocator)(void)) {
  DCHECK(broker_sandboxer_allocator);
  UpdateProcessTypeToGpuBroker();
  return SandboxSeccompBPF::StartSandboxWithExternalPolicy(
      make_scoped_ptr(broker_sandboxer_allocator()), base::ScopedFD());
}
