void TextureMapperGL::drawTexture(uint32_t texture, bool opaque, const FloatSize& relativeSize, const FloatRect& targetRect, const TransformationMatrix& modelViewMatrix, float opacity, const BitmapTexture* maskTexture, bool flip)
{
    TextureMapperGLData::GlobalGLData::ShaderProgramIndex program;
    if (maskTexture)
        program = TextureMapperGLData::GlobalGLData::OpacityAndMaskProgram;
    else
        program = TextureMapperGLData::GlobalGLData::SimpleProgram;

    const TextureMapperGLData::GlobalGLData::ProgramInfo& programInfo = data().globalGLData.programs[program];
    GL_CMD(glUseProgram(programInfo.id))
    data().currentProgram = program;
    GL_CMD(glEnableVertexAttribArray(programInfo.vertexAttrib))
    GL_CMD(glActiveTexture(GL_TEXTURE0))
    GL_CMD(glBindTexture(GL_TEXTURE_2D, texture))
    GL_CMD(glBindBuffer(GL_ARRAY_BUFFER, 0))
    const GLfloat unitRect[] = {0, 0, 1, 0, 1, 1, 0, 1};
    GL_CMD(glVertexAttribPointer(programInfo.vertexAttrib, 2, GL_FLOAT, GL_FALSE, 0, unitRect))

    TransformationMatrix matrix = TransformationMatrix(data().projectionMatrix).multiply(modelViewMatrix).multiply(TransformationMatrix(
            targetRect.width(), 0, 0, 0,
            0, targetRect.height(), 0, 0,
            0, 0, 1, 0,
            targetRect.x(), targetRect.y(), 0, 1));

    const GLfloat m4[] = {
        matrix.m11(), matrix.m12(), matrix.m13(), matrix.m14(),
        matrix.m21(), matrix.m22(), matrix.m23(), matrix.m24(),
        matrix.m31(), matrix.m32(), matrix.m33(), matrix.m34(),
        matrix.m41(), matrix.m42(), matrix.m43(), matrix.m44()
    };
    const GLfloat m4src[] = {relativeSize.width(), 0, 0, 0,
                                     0, relativeSize.height() * (flip ? -1 : 1), 0, 0,
                                     0, 0, 1, 0,
                                     0, flip ? relativeSize.height() : 0, 0, 1};

    GL_CMD(glUniformMatrix4fv(programInfo.vars[TextureMapperGLData::GlobalGLData::InMatrixVariable], 1, GL_FALSE, m4))
    GL_CMD(glUniformMatrix4fv(programInfo.vars[TextureMapperGLData::GlobalGLData::InSourceMatrixVariable], 1, GL_FALSE, m4src))
    GL_CMD(glUniform1i(programInfo.vars[TextureMapperGLData::GlobalGLData::SourceTextureVariable], 0))
    GL_CMD(glUniform1f(programInfo.vars[TextureMapperGLData::GlobalGLData::OpacityVariable], opacity))

    if (maskTexture && maskTexture->isValid()) {
        const BitmapTextureGL* maskTextureGL = static_cast<const BitmapTextureGL*>(maskTexture);
        GL_CMD(glActiveTexture(GL_TEXTURE1))
        GL_CMD(glBindTexture(GL_TEXTURE_2D, maskTextureGL->id()))
        const GLfloat m4mask[] = {maskTextureGL->relativeSize().width(), 0, 0, 0,
                                         0, maskTextureGL->relativeSize().height(), 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1};
        GL_CMD(glUniformMatrix4fv(programInfo.vars[TextureMapperGLData::GlobalGLData::InMaskMatrixVariable], 1, GL_FALSE, m4mask));
        GL_CMD(glUniform1i(programInfo.vars[TextureMapperGLData::GlobalGLData::MaskTextureVariable], 1))
        GL_CMD(glActiveTexture(GL_TEXTURE0))
    }

    if (opaque && opacity > 0.99 && !maskTexture)
        GL_CMD(glDisable(GL_BLEND))
    else {
        GL_CMD(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA))
        GL_CMD(glEnable(GL_BLEND))
    }

    GL_CMD(glDisable(GL_DEPTH_TEST))
    GL_CMD(glDrawArrays(GL_TRIANGLE_FAN, 0, 4))
    GL_CMD(glDisableVertexAttribArray(programInfo.vertexAttrib))
}
