size_t GraphicBuffer::getFdCount() const {
 return handle ? handle->numFds : 0;
}
