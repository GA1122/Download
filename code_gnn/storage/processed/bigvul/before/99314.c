void ResourceMessageFilter::OnKeygen(uint32 key_size_index,
                                     const std::string& challenge_string,
                                     const GURL& url,
                                     std::string* signed_public_key) {
  int key_size_in_bits;
  switch (key_size_index) {
    case 0:
      key_size_in_bits = 2048;
      break;
    case 1:
      key_size_in_bits = 1024;
      break;
    default:
      DCHECK(false) << "Illegal key_size_index " << key_size_index;
      *signed_public_key = std::string();
      return;
  }
  net::KeygenHandler keygen_handler(key_size_in_bits, challenge_string);
  *signed_public_key = keygen_handler.GenKeyAndSignChallenge();
}
