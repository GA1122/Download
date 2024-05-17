  bool StateEquals(int index, const State& state) {
    State s = GetStateAt(index);
    return (s.src_contents == state.src_contents &&
            s.dst_contents == state.dst_contents &&
            s.src_index == state.src_index &&
            s.dst_index == state.dst_index &&
            s.user_gesture == state.user_gesture &&
            s.foreground == state.foreground &&
            s.action == state.action);
  }
