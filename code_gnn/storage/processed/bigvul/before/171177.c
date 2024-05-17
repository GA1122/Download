bool MediaPlayerService::AudioOutput::isOnEmulator()
{
    setMinBufferCount();  
 return mIsOnEmulator;
}
