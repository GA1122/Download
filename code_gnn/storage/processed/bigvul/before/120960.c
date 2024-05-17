void SocketStream::Delegate::OnAuthRequired(SocketStream* socket,
                                            AuthChallengeInfo* auth_info) {
  socket->Close();
}
