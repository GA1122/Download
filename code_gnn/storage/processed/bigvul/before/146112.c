void WebGL2RenderingContextBase::invalidateSubFramebuffer(
    GLenum target,
    const Vector<GLenum>& attachments,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height) {
  if (isContextLost())
    return;

  Vector<GLenum> translated_attachments = attachments;
  if (!CheckAndTranslateAttachments("invalidateSubFramebuffer", target,
                                    translated_attachments))
    return;
  ContextGL()->InvalidateSubFramebuffer(target, translated_attachments.size(),
                                        translated_attachments.data(), x, y,
                                        width, height);
}
