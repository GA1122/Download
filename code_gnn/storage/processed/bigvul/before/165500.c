void ContentSecurityPolicy::ApplyPolicySideEffectsToExecutionContext() {
  DCHECK(execution_context_ &&
         execution_context_->GetSecurityContext().GetSecurityOrigin());
  SecurityContext& security_context = execution_context_->GetSecurityContext();

  SetupSelf(*security_context.GetSecurityOrigin());

  Document* document = this->GetDocument();
  if (sandbox_mask_ != kSandboxNone) {
    UseCounter::Count(execution_context_, WebFeature::kSandboxViaCSP);
    if (document)
      document->EnforceSandboxFlags(sandbox_mask_);
    else
      security_context.ApplySandboxFlags(sandbox_mask_);
  }
  if (treat_as_public_address_) {
    security_context.SetAddressSpace(mojom::IPAddressSpace::kPublic);
  }
  if (require_safe_types_) {
    security_context.SetRequireTrustedTypes();
  }

  security_context.SetInsecureRequestPolicy(
      security_context.GetInsecureRequestPolicy() | insecure_request_policy_);
  if (document)
    document->DidEnforceInsecureRequestPolicy();

  if (insecure_request_policy_ & kUpgradeInsecureRequests) {
    UseCounter::Count(execution_context_,
                      WebFeature::kUpgradeInsecureRequestsEnabled);
    if (!execution_context_->Url().Host().IsEmpty()) {
      uint32_t hash = execution_context_->Url().Host().Impl()->GetHash();
      security_context.AddInsecureNavigationUpgrade(hash);
      if (document)
        document->DidEnforceInsecureNavigationsSet();
    }
  }

  for (const auto& console_message : console_messages_)
    execution_context_->AddConsoleMessage(console_message);
  console_messages_.clear();

  for (const auto& policy : policies_) {
    UseCounter::Count(execution_context_,
                      GetUseCounterType(policy->HeaderType()));
    if (policy->AllowDynamic(
            ContentSecurityPolicy::DirectiveType::kScriptSrcAttr) ||
        policy->AllowDynamic(
            ContentSecurityPolicy::DirectiveType::kScriptSrcElem)) {
      UseCounter::Count(execution_context_, WebFeature::kCSPWithStrictDynamic);
    }

    if (policy->AllowEval(nullptr,
                          SecurityViolationReportingPolicy::kSuppressReporting,
                          kWillNotThrowException, g_empty_string)) {
      UseCounter::Count(execution_context_, WebFeature::kCSPWithUnsafeEval);
    }
  }

  if (!disable_eval_error_message_.IsNull())
    execution_context_->DisableEval(disable_eval_error_message_);
}
