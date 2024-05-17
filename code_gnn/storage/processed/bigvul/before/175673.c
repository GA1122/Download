void FLACParser::allocateBuffers()
{
    CHECK(mGroup == NULL);
    mGroup = new MediaBufferGroup;
    mMaxBufferSize = getMaxBlockSize() * getChannels() * sizeof(short);
    mGroup->add_buffer(new MediaBuffer(mMaxBufferSize));
}
