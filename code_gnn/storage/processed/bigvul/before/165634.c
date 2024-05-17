String Location::toString() const {
  USVStringOrTrustedURL result;
  href(result);
  DCHECK(result.IsUSVString());
  return result.GetAsUSVString();
}
