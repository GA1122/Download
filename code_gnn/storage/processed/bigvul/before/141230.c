void SetInitialInterventionPolicies(FrameResourceCoordinator* frame_coordinator,
                                    const ExecutionContext* context) {
  for (size_t i = 0; i < base::size(kInterventionPolicyGetters); ++i) {
    bool opt_in = (*kInterventionPolicyGetters[i].opt_in_getter)(context);
    bool opt_out = (*kInterventionPolicyGetters[i].opt_out_getter)(context);

    InterventionPolicy policy = InterventionPolicy::kDefault;
    if (opt_out)
      policy = InterventionPolicy::kOptOut;
    else if (opt_in)
      policy = InterventionPolicy::kOptIn;

    frame_coordinator->SetInterventionPolicy(
        static_cast<PolicyControlledIntervention>(i), policy);
  }
}
