void DrawingBuffer::ForceNextDrawingBufferCreationToFail() {
  g_should_fail_drawing_buffer_creation_for_testing = true;
}
