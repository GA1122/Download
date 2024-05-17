        GLuint findOrCreate(ImageUID image, bool& found)
        {
            ImageTextureMap::iterator it = imageToTexture.find(image);
            found = false;
            if (it != imageToTexture.end()) {
                it->second.refCount++;
                found = true;
                return it->second.texture;
            }
            Entry entry;
            GL_CMD(glGenTextures(1, &entry.texture));
            entry.refCount = 1;
            imageToTexture.add(image, entry);
            return entry.texture;
        }
