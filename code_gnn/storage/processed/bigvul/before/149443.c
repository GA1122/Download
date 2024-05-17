std::unique_ptr<Vector<CSPHeaderAndType>> ContentSecurityPolicy::headers()
    const {
  std::unique_ptr<Vector<CSPHeaderAndType>> headers =
      WTF::wrapUnique(new Vector<CSPHeaderAndType>);
  for (const auto& policy : m_policies) {
    CSPHeaderAndType headerAndType(policy->header(), policy->headerType());
    headers->push_back(headerAndType);
  }
  return headers;
}
