void ContentSecurityPolicy::AddPolicyFromHeaderValue(
    const String& header,
    ContentSecurityPolicyHeaderType type,
    ContentSecurityPolicyHeaderSource source) {
  if (source == kContentSecurityPolicyHeaderSourceMeta &&
      type == kContentSecurityPolicyHeaderTypeReport) {
    ReportReportOnlyInMeta(header);
    return;
  }

  if (source == kContentSecurityPolicyHeaderSourceHTTP)
    header_delivered_ = true;

  Vector<UChar> characters;
  header.AppendTo(characters);

  const UChar* begin = characters.data();
  const UChar* end = begin + characters.size();

  const UChar* position = begin;
  while (position < end) {
    SkipUntil<UChar>(position, end, ',');

    Member<CSPDirectiveList> policy =
        CSPDirectiveList::Create(this, begin, position, type, source);

    if (!policy->AllowEval(nullptr,
                           SecurityViolationReportingPolicy::kSuppressReporting,
                           kWillNotThrowException, g_empty_string) &&
        disable_eval_error_message_.IsNull()) {
      disable_eval_error_message_ = policy->EvalDisabledErrorMessage();
    }

    policies_.push_back(policy.Release());

    DCHECK(position == end || *position == ',');
    SkipExactly<UChar>(position, end, ',');
    begin = position;
  }
}
