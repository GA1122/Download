status_t ACodec::setComponentRole(
 bool isEncoder, const char *mime) {
 struct MimeToRole {
 const char *mime;
 const char *decoderRole;
 const char *encoderRole;
 };

 static const MimeToRole kMimeToRole[] = {
 { MEDIA_MIMETYPE_AUDIO_MPEG,
 "audio_decoder.mp3", "audio_encoder.mp3" },
 { MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_I,
 "audio_decoder.mp1", "audio_encoder.mp1" },
 { MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II,
 "audio_decoder.mp2", "audio_encoder.mp2" },
 { MEDIA_MIMETYPE_AUDIO_AMR_NB,
 "audio_decoder.amrnb", "audio_encoder.amrnb" },
 { MEDIA_MIMETYPE_AUDIO_AMR_WB,
 "audio_decoder.amrwb", "audio_encoder.amrwb" },
 { MEDIA_MIMETYPE_AUDIO_AAC,
 "audio_decoder.aac", "audio_encoder.aac" },
 { MEDIA_MIMETYPE_AUDIO_VORBIS,
 "audio_decoder.vorbis", "audio_encoder.vorbis" },
 { MEDIA_MIMETYPE_AUDIO_OPUS,
 "audio_decoder.opus", "audio_encoder.opus" },
 { MEDIA_MIMETYPE_AUDIO_G711_MLAW,
 "audio_decoder.g711mlaw", "audio_encoder.g711mlaw" },
 { MEDIA_MIMETYPE_AUDIO_G711_ALAW,
 "audio_decoder.g711alaw", "audio_encoder.g711alaw" },
 { MEDIA_MIMETYPE_VIDEO_AVC,
 "video_decoder.avc", "video_encoder.avc" },
 { MEDIA_MIMETYPE_VIDEO_HEVC,
 "video_decoder.hevc", "video_encoder.hevc" },
 { MEDIA_MIMETYPE_VIDEO_MPEG4,
 "video_decoder.mpeg4", "video_encoder.mpeg4" },
 { MEDIA_MIMETYPE_VIDEO_H263,
 "video_decoder.h263", "video_encoder.h263" },
 { MEDIA_MIMETYPE_VIDEO_VP8,
 "video_decoder.vp8", "video_encoder.vp8" },
 { MEDIA_MIMETYPE_VIDEO_VP9,
 "video_decoder.vp9", "video_encoder.vp9" },
 { MEDIA_MIMETYPE_AUDIO_RAW,
 "audio_decoder.raw", "audio_encoder.raw" },
 { MEDIA_MIMETYPE_AUDIO_FLAC,
 "audio_decoder.flac", "audio_encoder.flac" },
 { MEDIA_MIMETYPE_AUDIO_MSGSM,
 "audio_decoder.gsm", "audio_encoder.gsm" },
 { MEDIA_MIMETYPE_VIDEO_MPEG2,
 "video_decoder.mpeg2", "video_encoder.mpeg2" },
 { MEDIA_MIMETYPE_AUDIO_AC3,
 "audio_decoder.ac3", "audio_encoder.ac3" },
 { MEDIA_MIMETYPE_AUDIO_EAC3,
 "audio_decoder.eac3", "audio_encoder.eac3" },
 };

 static const size_t kNumMimeToRole =
 sizeof(kMimeToRole) / sizeof(kMimeToRole[0]);

 size_t i;
 for (i = 0; i < kNumMimeToRole; ++i) {
 if (!strcasecmp(mime, kMimeToRole[i].mime)) {
 break;
 }
 }

 if (i == kNumMimeToRole) {
 return ERROR_UNSUPPORTED;
 }

 const char *role =
        isEncoder ? kMimeToRole[i].encoderRole
 : kMimeToRole[i].decoderRole;

 if (role != NULL) {
        OMX_PARAM_COMPONENTROLETYPE roleParams;
 InitOMXParams(&roleParams);

        strncpy((char *)roleParams.cRole,
                role, OMX_MAX_STRINGNAME_SIZE - 1);

        roleParams.cRole[OMX_MAX_STRINGNAME_SIZE - 1] = '\0';

 status_t err = mOMX->setParameter(
                mNode, OMX_IndexParamStandardComponentRole,
 &roleParams, sizeof(roleParams));

 if (err != OK) {
            ALOGW("[%s] Failed to set standard component role '%s'.",
                 mComponentName.c_str(), role);

 return err;
 }
 }

 return OK;
}
