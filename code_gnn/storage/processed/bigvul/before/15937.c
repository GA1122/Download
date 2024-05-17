void Container::write(WEBP_MetaHandler* handler)
{
    XMP_IO* file = handler->parent->ioRef;
    file->Rewind();
    XIO::WriteUns32_LE(file, this->tag);
    XIO::WriteUns32_LE(file, (XMP_Uns32) this->size);
    XIO::WriteUns32_LE(file, kChunk_WEBP);

    size_t i, j;
    std::vector<Chunk*> chunkVect;
    for (i = 0; i < WEBP_CHUNK_NIL; i++) {
        chunkVect = this->chunks[i];
        for (j = 0; j < chunkVect.size(); j++) {
            chunkVect.at(j)->write(handler);
        }
    }
    XMP_Int64 lastOffset = file->Offset();
    this->size = lastOffset - 8;
    file->Seek(this->pos + 4, kXMP_SeekFromStart);
    XIO::WriteUns32_LE(file, (XMP_Uns32) this->size);
    file->Seek(lastOffset, kXMP_SeekFromStart);
    if (lastOffset < handler->initialFileSize) {
        file->Truncate(lastOffset);
    }
}
