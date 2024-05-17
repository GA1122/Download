void TabletEventConverterEvdev::ProcessEvents(const input_event* inputs,
                                              int count) {
  for (int i = 0; i < count; ++i) {
    const input_event& input = inputs[i];
    switch (input.type) {
      case EV_KEY:
        ConvertKeyEvent(input);
        break;
      case EV_ABS:
        ConvertAbsEvent(input);
        break;
      case EV_SYN:
        FlushEvents(input);
        break;
    }
  }
}
