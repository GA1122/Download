  explicit AndroidHostDisplayClient(
      base::RepeatingCallback<void(const gfx::Size&)> on_swap,
      base::RepeatingCallback<void(gpu::ContextResult)>
          on_context_creation_failure)
      : HostDisplayClient(gfx::kNullAcceleratedWidget),
        on_swap_(std::move(on_swap)),
        on_context_creation_failure_(std::move(on_context_creation_failure)) {}
