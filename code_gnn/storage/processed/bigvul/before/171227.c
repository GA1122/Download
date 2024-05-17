MediaPlayerService::AudioOutput::~AudioOutput()
{
    close();
    free(mAttributes);
 delete mCallbackData;
}
