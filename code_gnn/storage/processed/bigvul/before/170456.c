bool Parcel::pushAllowFds(bool allowFds)
{
 const bool origValue = mAllowFds;
 if (!allowFds) {
        mAllowFds = false;
 }
 return origValue;
}
