        bool deref(ImageUID image)
        {
            HashMap<ImageUID, Entry>::iterator it = imageToTexture.find(image);
            if (it != imageToTexture.end()) {
                if (it->second.refCount < 2) {
                    imageToTexture.remove(it);
                    return false;
                }
            }
            return true;
        }
