  bool StateEquals(int index, const State& state) {
    State* s = GetStateAt(index);
    EXPECT_EQ(state.src_contents, s->src_contents);
    EXPECT_EQ(state.dst_contents, s->dst_contents);
    EXPECT_EQ(state.src_index, s->src_index);
    EXPECT_EQ(state.dst_index, s->dst_index);
    EXPECT_EQ(state.user_gesture, s->user_gesture);
    EXPECT_EQ(state.foreground, s->foreground);
    EXPECT_EQ(state.action, s->action);
    return (s->src_contents == state.src_contents &&
            s->dst_contents == state.dst_contents &&
            s->src_index == state.src_index &&
            s->dst_index == state.dst_index &&
            s->user_gesture == state.user_gesture &&
            s->foreground == state.foreground &&
            s->action == state.action);
  }
