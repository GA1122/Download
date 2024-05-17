  void DoTexStorageFormatAndType(const InitState& init,
                                 GLenum format,
                                 GLenum adjusted_internal_format) {
    GLsizei kWidth = 512;
    GLsizei kHeight = 512;
    GLsizei kLevels = 5;
    InitDecoder(init);
    DoBindTexture(GL_TEXTURE_2D, client_texture_id_, kServiceTextureId);
    EXPECT_CALL(
        *gl_, TexStorage2DEXT(GL_TEXTURE_2D, kLevels, format, kWidth, kHeight))
        .Times(1)
        .RetiresOnSaturation();
    TexStorage2DEXT cmd;
    cmd.Init(GL_TEXTURE_2D, kLevels, format, kWidth, kHeight);
    EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
    EXPECT_EQ(GL_NO_ERROR, GetGLError());
    TextureRef* texture_ref =
        group().texture_manager()->GetTexture(client_texture_id_);
    Texture* texture = texture_ref->texture();
    for (GLsizei ii = 0; ii < kLevels; ++ii) {
      GLenum type = 0, internal_format = 0;
      GLsizei level_width = 0, level_height = 0;
      EXPECT_TRUE(texture->GetLevelType(GL_TEXTURE_2D, static_cast<GLint>(ii),
                                        &type, &internal_format));
      EXPECT_EQ(static_cast<GLenum>(adjusted_internal_format), internal_format);
      EXPECT_EQ(static_cast<GLenum>(GL_UNSIGNED_BYTE), type);
      EXPECT_TRUE(texture->GetLevelSize(GL_TEXTURE_2D, static_cast<GLint>(ii),
                                        &level_width, &level_height, nullptr));
      EXPECT_EQ(kWidth >> ii, level_width);
      EXPECT_EQ(kHeight >> ii, level_height);
    }
    EXPECT_TRUE(texture->texture_complete());
  }
