ContentSecurityPolicy::ContentSecurityPolicy()
    : m_executionContext(nullptr),
      m_overrideInlineStyleAllowed(false),
      m_scriptHashAlgorithmsUsed(ContentSecurityPolicyHashAlgorithmNone),
      m_styleHashAlgorithmsUsed(ContentSecurityPolicyHashAlgorithmNone),
      m_sandboxMask(0),
      m_treatAsPublicAddress(false),
      m_insecureRequestPolicy(kLeaveInsecureRequestsAlone) {}
