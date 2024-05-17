 GLHelper* CmdBufferImageTransportFactory::GetGLHelper() {
   if (!gl_helper_.get())
     gl_helper_.reset(new GLHelper(GetContext3D(), NULL));

  return gl_helper_.get();
}
