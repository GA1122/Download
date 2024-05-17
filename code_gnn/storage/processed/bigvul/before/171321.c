bool OMXCodec::drainInputBuffer(IOMX::buffer_id buffer) {
 Vector<BufferInfo> *buffers = &mPortBuffers[kPortIndexInput];
 for (size_t i = 0; i < buffers->size(); ++i) {
 if ((*buffers)[i].mBuffer == buffer) {
 return drainInputBuffer(&buffers->editItemAt(i));
 }
 }

    CHECK(!"should not be here.");

 return false;
}
