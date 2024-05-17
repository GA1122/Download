void NtlmClient::CalculatePayloadLayout(
    bool is_unicode,
    const base::string16& domain,
    const base::string16& username,
    const std::string& hostname,
    size_t updated_target_info_len,
    SecurityBuffer* lm_info,
    SecurityBuffer* ntlm_info,
    SecurityBuffer* domain_info,
    SecurityBuffer* username_info,
    SecurityBuffer* hostname_info,
    SecurityBuffer* session_key_info,
    size_t* authenticate_message_len) const {
  size_t upto = GetAuthenticateHeaderLength();

  session_key_info->offset = upto;
  session_key_info->length = 0;
  upto += session_key_info->length;

  lm_info->offset = upto;
  lm_info->length = kResponseLenV1;
  upto += lm_info->length;

  ntlm_info->offset = upto;
  ntlm_info->length = GetNtlmResponseLength(updated_target_info_len);
  upto += ntlm_info->length;

  domain_info->offset = upto;
  domain_info->length = GetStringPayloadLength(domain, is_unicode);
  upto += domain_info->length;

  username_info->offset = upto;
  username_info->length = GetStringPayloadLength(username, is_unicode);
  upto += username_info->length;

  hostname_info->offset = upto;
  hostname_info->length = GetStringPayloadLength(hostname, is_unicode);
  upto += hostname_info->length;

  *authenticate_message_len = upto;
}
