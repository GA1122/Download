void Location::href(USVStringOrTrustedURL& result) const {
  result.SetUSVString(Url().StrippedForUseAsHref());
}
