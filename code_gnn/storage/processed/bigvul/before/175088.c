void SoundPool::setCallback(SoundPoolCallback* callback, void* user)
{
 Mutex::Autolock lock(&mCallbackLock);
    mCallback = callback;
    mUserData = user;
}
