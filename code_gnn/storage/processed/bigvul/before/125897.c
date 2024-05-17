QuicPacket* ConstructHandshakePacket(QuicGuid guid, CryptoTag tag) {
  CryptoHandshakeMessage message;
  message.tag = tag;
  CryptoFramer crypto_framer;
  scoped_ptr<QuicData> data(crypto_framer.ConstructHandshakeMessage(message));
  QuicFramer quic_framer(QuicDecrypter::Create(kNULL),
                         QuicEncrypter::Create(kNULL));

  QuicPacketHeader header;
  header.guid = guid;
  header.retransmission_count = 0;
  header.packet_sequence_number = 1;
  header.transmission_time = 0;
  header.flags = PACKET_FLAGS_NONE;
  header.fec_group = 0;

  QuicStreamFrame stream_frame(kCryptoStreamId, false, 0,
                                     data->AsStringPiece());

  QuicFrame frame(&stream_frame);
  QuicFrames frames;
  frames.push_back(frame);
  QuicPacket* packet;
  quic_framer.ConstructFragementDataPacket(header, frames, &packet);
   return packet;
 }
