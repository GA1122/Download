Chunk* Container::getExifChunk()
{
    if (this->chunks[WEBP::WEBP_CHUNK_EXIF].size() == 0) {
        return NULL;
    }
    return this->chunks[WEBP::WEBP_CHUNK_EXIF][0];
}
