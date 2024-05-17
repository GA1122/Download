void GLES2DecoderImpl::CompileShaderAndExitCommandProcessingEarly(
    Shader* shader) {
  if (!shader->CanCompile())
    return;

  shader->DoCompile();

  ExitCommandProcessingEarly();
}
