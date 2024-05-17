void ContentSecurityPolicy::addPolicyFromHeaderValue(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  if (source == ContentSecurityPolicyHeaderSourceMeta &&
      type == ContentSecurityPolicyHeaderTypeReport) {
    reportReportOnlyInMeta(header);
    return;
  }

  Vector<UChar> characters;
  header.appendTo(characters);

  const UChar* begin = characters.data();
  const UChar* end = begin + characters.size();

  const UChar* position = begin;
  while (position < end) {
    skipUntil<UChar>(position, end, ',');

    Member<CSPDirectiveList> policy =
        CSPDirectiveList::create(this, begin, position, type, source);

    if (!policy->allowEval(
            0, SecurityViolationReportingPolicy::SuppressReporting) &&
        m_disableEvalErrorMessage.isNull())
      m_disableEvalErrorMessage = policy->evalDisabledErrorMessage();

    m_policies.push_back(policy.release());

    ASSERT(position == end || *position == ',');
    skipExactly<UChar>(position, end, ',');
    begin = position;
  }
}
