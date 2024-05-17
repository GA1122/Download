std::string SecurityStyleToProtocolSecurityState(
    blink::WebSecurityStyle security_style) {
  switch (security_style) {
    case blink::kWebSecurityStyleUnknown:
      return Security::SecurityStateEnum::Unknown;
    case blink::kWebSecurityStyleNeutral:
      return Security::SecurityStateEnum::Neutral;
    case blink::kWebSecurityStyleInsecure:
      return Security::SecurityStateEnum::Insecure;
    case blink::kWebSecurityStyleSecure:
      return Security::SecurityStateEnum::Secure;
    default:
      NOTREACHED();
      return Security::SecurityStateEnum::Unknown;
  }
}
