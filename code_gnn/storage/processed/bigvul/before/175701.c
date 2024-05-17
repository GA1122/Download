void FLACParser::releaseBuffers()
{
    CHECK(mGroup != NULL);
 delete mGroup;
    mGroup = NULL;
}
