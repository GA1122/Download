        ~DirectlyCompositedImageRepository()
        {
            for (ImageTextureMap::iterator it = imageToTexture.begin(); it != imageToTexture.end(); ++it) {
                GLuint texture = it->second.texture;
                if (texture)
                    GL_CMD(glDeleteTextures(1, &texture));
            }

        }
