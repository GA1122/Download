bool ContextualSearchFieldTrial::IsDecodeMentionsDisabled() {
  return GetBooleanParam(kContextualSearchDecodeMentionsDisabledParamName,
                         &is_decode_mentions_disabled_cached_,
                          &is_decode_mentions_disabled_);
 }
