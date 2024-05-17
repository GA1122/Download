SoundTriggerHwService::Model::Model(sound_model_handle_t handle, audio_session_t session,
 audio_io_handle_t ioHandle, audio_devices_t device,
 sound_trigger_sound_model_type_t type) :
    mHandle(handle), mState(STATE_IDLE), mCaptureSession(session),
    mCaptureIOHandle(ioHandle), mCaptureDevice(device), mType(type)
{

}
