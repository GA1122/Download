CryptohomeClient* CryptohomeClient::Create() {
  return new CryptohomeClientImpl();
}
