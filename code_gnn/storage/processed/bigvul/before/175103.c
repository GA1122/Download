SoundPool::~SoundPool()
{
    ALOGV("SoundPool destructor");
    mDecodeThread->quit();
    quit();

 Mutex::Autolock lock(&mLock);

    mChannels.clear();
 if (mChannelPool)
 delete [] mChannelPool;
    ALOGV("clear samples");
    mSamples.clear();

 if (mDecodeThread)
 delete mDecodeThread;
}
