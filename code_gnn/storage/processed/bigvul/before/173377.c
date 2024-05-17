void Parcel::setDataPosition(size_t pos) const
{
 if (pos > INT32_MAX) {
        abort();
 }

    mDataPos = pos;
    mNextObjectHint = 0;
}
