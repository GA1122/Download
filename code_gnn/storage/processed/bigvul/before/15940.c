Container::~Container()
{
    Chunk* chunk;
    size_t i;
    std::vector<Chunk*> chunkVect;
    for (i = 0; i < WEBP_CHUNK_NIL; i++) {
        chunkVect = this->chunks[i];
        while (!chunkVect.empty()) {
            chunk = chunkVect.back();
            delete chunk;
            chunkVect.pop_back();
        }
    }
}
