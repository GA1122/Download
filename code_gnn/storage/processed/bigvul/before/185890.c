 static bool ExecuteTranspose(LocalFrame& frame,
                               Event*,
                               EditorCommandSource,
                               const String&) {
  frame.GetEditor().Transpose();
//   Transpose(frame);
    return true;
  }