void TextureMapperGL::beginClip(const TransformationMatrix& modelViewMatrix, const FloatRect& targetRect)
{
    TextureMapperGLData::GlobalGLData::ShaderProgramIndex program = TextureMapperGLData::GlobalGLData::ClipProgram;
    const TextureMapperGLData::GlobalGLData::ProgramInfo& programInfo = data().globalGLData.programs[program];
    GL_CMD(glUseProgram(programInfo.id))
    GL_CMD(glEnableVertexAttribArray(programInfo.vertexAttrib))
    const GLfloat unitRect[] = {0, 0, 1, 0, 1, 1, 0, 1};
    GL_CMD(glVertexAttribPointer(programInfo.vertexAttrib, 2, GL_FLOAT, GL_FALSE, 0, unitRect))

    TransformationMatrix matrix = TransformationMatrix(data().projectionMatrix)
            .multiply(modelViewMatrix)
            .multiply(TransformationMatrix(targetRect.width(), 0, 0, 0,
                0, targetRect.height(), 0, 0,
                0, 0, 1, 0,
                targetRect.x(), targetRect.y(), 0, 1));

    const GLfloat m4[] = {
        matrix.m11(), matrix.m12(), matrix.m13(), matrix.m14(),
        matrix.m21(), matrix.m22(), matrix.m23(), matrix.m24(),
        matrix.m31(), matrix.m32(), matrix.m33(), matrix.m34(),
        matrix.m41(), matrix.m42(), matrix.m43(), matrix.m44()
    };

    int& stencilIndex = data().globalGLData.stencilIndex;

    GL_CMD(glUniformMatrix4fv(programInfo.vars[TextureMapperGLData::GlobalGLData::InMatrixVariable], 1, GL_FALSE, m4))
    GL_CMD(glEnable(GL_STENCIL_TEST))
    GL_CMD(glStencilFunc(GL_NEVER, stencilIndex, stencilIndex))
    GL_CMD(glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE))
    GL_CMD(glStencilMask(0xff & ~(stencilIndex - 1)))
    GL_CMD(glDrawArrays(GL_TRIANGLE_FAN, 0, 4))
    GL_CMD(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP))
    stencilIndex <<= 1;
    glStencilFunc(stencilIndex > 1 ? GL_EQUAL : GL_ALWAYS, stencilIndex - 1, stencilIndex - 1);
    GL_CMD(glDisableVertexAttribArray(programInfo.vertexAttrib))
}
