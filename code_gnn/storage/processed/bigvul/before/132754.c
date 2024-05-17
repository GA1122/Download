void ChromotingInstance::SetPairingResponse(
    const protocol::PairingResponse& pairing_response) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("clientId", pairing_response.client_id());
  data->SetString("sharedSecret", pairing_response.shared_secret());
  PostLegacyJsonMessage("pairingResponse", data.Pass());
}
