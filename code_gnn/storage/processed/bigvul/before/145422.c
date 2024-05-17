  FakePacketTransport(quic::QuicAlarmFactory* alarm_factory,
                      quic::MockClock* clock)
      : alarm_(alarm_factory->CreateAlarm(new AlarmDelegate(this))),
        clock_(clock) {}
