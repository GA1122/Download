  void CheckFormats(const char* extension, const GLenum* formats, int count) {
    InitState init;
    init.extensions = extension;
    init.bind_generates_resource = true;
    InitDecoder(init);

    EXPECT_CALL(*gl_, GetError())
        .WillOnce(Return(GL_NO_ERROR))
        .WillOnce(Return(GL_NO_ERROR))
        .WillOnce(Return(GL_NO_ERROR))
        .WillOnce(Return(GL_NO_ERROR))
        .RetiresOnSaturation();

    typedef GetIntegerv::Result Result;
    Result* result = static_cast<Result*>(shared_memory_address_);
    GetIntegerv cmd;
    result->size = 0;
    EXPECT_CALL(*gl_, GetIntegerv(_, _)).Times(0).RetiresOnSaturation();
    cmd.Init(GL_NUM_COMPRESSED_TEXTURE_FORMATS,
             shared_memory_id_,
             shared_memory_offset_);
    EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
    EXPECT_EQ(1, result->GetNumResults());
    GLint num_formats = result->GetData()[0];
    EXPECT_EQ(count, num_formats);
    EXPECT_EQ(GL_NO_ERROR, GetGLError());

    result->size = 0;
    cmd.Init(GL_COMPRESSED_TEXTURE_FORMATS,
             shared_memory_id_,
             shared_memory_offset_);
    EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));
    EXPECT_EQ(num_formats, result->GetNumResults());

    for (int i = 0; i < count; ++i) {
      EXPECT_TRUE(
          ValueInArray(formats[i], result->GetData(), result->GetNumResults()));
    }

    EXPECT_EQ(GL_NO_ERROR, GetGLError());
  }
