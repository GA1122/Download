void Container::addChunk(Chunk* chunk)
{
    ChunkId idx;

    try {
        idx = chunkMap.at(chunk->tag);
    }
    catch (const std::out_of_range& e) {
        idx = WEBP_CHUNK_UNKNOWN;
    }
    this->chunks[idx].push_back(chunk);
}
