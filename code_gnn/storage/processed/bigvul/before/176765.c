status_t Parcel::readUniqueFileDescriptor(ScopedFd* val) const
{
 int got = readFileDescriptor();

 if (got == BAD_TYPE) {
 return BAD_TYPE;
 }

    val->reset(dup(got));

 if (val->get() < 0) {
 return BAD_VALUE;
 }

 return OK;
}
