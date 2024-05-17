  Buffer* GetBuffer(GLuint client_id) {
    Buffer* buffer = buffer_manager()->GetBuffer(client_id);
    return buffer;
  }
