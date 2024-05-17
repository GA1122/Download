  SocketStreamEvent(EventType type,
                    SocketStream* socket_stream,
                    int num,
                    const std::string& str,
                    AuthChallengeInfo* auth_challenge_info,
                    int error)
      : event_type(type), socket(socket_stream), number(num), data(str),
        auth_info(auth_challenge_info), error_code(error) {}
