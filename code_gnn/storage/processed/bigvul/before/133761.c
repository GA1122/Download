SSLClientSocket::NextProtoStatus SSLClientSocketOpenSSL::GetNextProto(
    std::string* proto, std::string* server_protos) {
  *proto = npn_proto_;
  *server_protos = server_protos_;
  return npn_status_;
}
