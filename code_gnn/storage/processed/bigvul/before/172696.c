void MediaRecorder::died()
{
    ALOGV("died");
    notify(MEDIA_RECORDER_EVENT_ERROR, MEDIA_ERROR_SERVER_DIED, 0);
}
