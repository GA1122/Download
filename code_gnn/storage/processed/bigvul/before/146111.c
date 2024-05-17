void WebGL2RenderingContextBase::invalidateFramebuffer(
    GLenum target,
    const Vector<GLenum>& attachments) {
  if (isContextLost())
    return;

  Vector<GLenum> translated_attachments = attachments;
  if (!CheckAndTranslateAttachments("invalidateFramebuffer", target,
                                    translated_attachments))
    return;
  ContextGL()->InvalidateFramebuffer(target, translated_attachments.size(),
                                     translated_attachments.data());
}
