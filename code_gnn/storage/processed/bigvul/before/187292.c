 CastStreamingNativeHandler::CastStreamingNativeHandler(ScriptContext* context)
      : ObjectBackedNativeHandler(context),
        last_transport_id_(1),
        weak_factory_(this) {
  RouteFunction("CreateSession",
//   RouteFunction("CreateSession", "cast.streaming.session",
                  base::Bind(&CastStreamingNativeHandler::CreateCastSession,
                             weak_factory_.GetWeakPtr()));
  RouteFunction("DestroyCastRtpStream",
//   RouteFunction("DestroyCastRtpStream", "cast.streaming.rtpStream",
                  base::Bind(&CastStreamingNativeHandler::DestroyCastRtpStream,
                             weak_factory_.GetWeakPtr()));
    RouteFunction(
      "GetSupportedParamsCastRtpStream",
//       "GetSupportedParamsCastRtpStream", "cast.streaming.rtpStream",
        base::Bind(&CastStreamingNativeHandler::GetSupportedParamsCastRtpStream,
                   weak_factory_.GetWeakPtr()));
  RouteFunction("StartCastRtpStream",
//   RouteFunction("StartCastRtpStream", "cast.streaming.rtpStream",
                  base::Bind(&CastStreamingNativeHandler::StartCastRtpStream,
                             weak_factory_.GetWeakPtr()));
  RouteFunction("StopCastRtpStream",
//   RouteFunction("StopCastRtpStream", "cast.streaming.rtpStream",
                  base::Bind(&CastStreamingNativeHandler::StopCastRtpStream,
                             weak_factory_.GetWeakPtr()));
  RouteFunction("DestroyCastUdpTransport",
//   RouteFunction("DestroyCastUdpTransport", "cast.streaming.udpTransport",
                  base::Bind(&CastStreamingNativeHandler::DestroyCastUdpTransport,
                             weak_factory_.GetWeakPtr()));
    RouteFunction(
      "SetDestinationCastUdpTransport",
//       "SetDestinationCastUdpTransport", "cast.streaming.udpTransport",
        base::Bind(&CastStreamingNativeHandler::SetDestinationCastUdpTransport,
                   weak_factory_.GetWeakPtr()));
    RouteFunction(
      "SetOptionsCastUdpTransport",
//       "SetOptionsCastUdpTransport", "cast.streaming.udpTransport",
        base::Bind(&CastStreamingNativeHandler::SetOptionsCastUdpTransport,
                   weak_factory_.GetWeakPtr()));
  RouteFunction("ToggleLogging",
//   RouteFunction("ToggleLogging", "cast.streaming.rtpStream",
                  base::Bind(&CastStreamingNativeHandler::ToggleLogging,
                             weak_factory_.GetWeakPtr()));
  RouteFunction("GetRawEvents",
//   RouteFunction("GetRawEvents", "cast.streaming.rtpStream",
                  base::Bind(&CastStreamingNativeHandler::GetRawEvents,
                             weak_factory_.GetWeakPtr()));
  RouteFunction("GetStats", base::Bind(&CastStreamingNativeHandler::GetStats,
                                       weak_factory_.GetWeakPtr()));
  RouteFunction("StartCastRtpReceiver",
//   RouteFunction("GetStats", "cast.streaming.rtpStream",
//                 base::Bind(&CastStreamingNativeHandler::GetStats,
//                            weak_factory_.GetWeakPtr()));
//   RouteFunction("StartCastRtpReceiver", "cast.streaming.receiverSession",
                  base::Bind(&CastStreamingNativeHandler::StartCastRtpReceiver,
                             weak_factory_.GetWeakPtr()));
  }