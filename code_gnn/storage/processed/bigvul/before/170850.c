size_t GraphicBuffer::getFlattenedSize() const {
 return (10 + (handle ? handle->numInts : 0))*sizeof(int);
}
