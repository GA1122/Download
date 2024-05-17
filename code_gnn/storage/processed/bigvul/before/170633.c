int AecSetDevice(preproc_effect_t *effect, uint32_t device)
{
    ALOGV("AecSetDevice %08x", device);
    webrtc::EchoControlMobile *aec = static_cast<webrtc::EchoControlMobile *>(effect->engine);
    webrtc::EchoControlMobile::RoutingMode mode = webrtc::EchoControlMobile::kQuietEarpieceOrHeadset;

 if (audio_is_input_device(device)) {
 return 0;
 }

 switch(device) {
 case AUDIO_DEVICE_OUT_EARPIECE:
        mode = webrtc::EchoControlMobile::kEarpiece;
 break;
 case AUDIO_DEVICE_OUT_SPEAKER:
        mode = webrtc::EchoControlMobile::kSpeakerphone;
 break;
 case AUDIO_DEVICE_OUT_WIRED_HEADSET:
 case AUDIO_DEVICE_OUT_WIRED_HEADPHONE:
 default:
 break;
 }
    aec->set_routing_mode(mode);
 return 0;
}
