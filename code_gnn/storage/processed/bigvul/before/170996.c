String8 effectFlagsToString(uint32_t flags) {
 String8 s;

    s.append("conn. mode: ");
 switch (flags & EFFECT_FLAG_TYPE_MASK) {
 case EFFECT_FLAG_TYPE_INSERT: s.append("insert"); break;
 case EFFECT_FLAG_TYPE_AUXILIARY: s.append("auxiliary"); break;
 case EFFECT_FLAG_TYPE_REPLACE: s.append("replace"); break;
 case EFFECT_FLAG_TYPE_PRE_PROC: s.append("preproc"); break;
 case EFFECT_FLAG_TYPE_POST_PROC: s.append("postproc"); break;
 default: s.append("unknown/reserved"); break;
 }
    s.append(", ");

    s.append("insert pref: ");
 switch (flags & EFFECT_FLAG_INSERT_MASK) {
 case EFFECT_FLAG_INSERT_ANY: s.append("any"); break;
 case EFFECT_FLAG_INSERT_FIRST: s.append("first"); break;
 case EFFECT_FLAG_INSERT_LAST: s.append("last"); break;
 case EFFECT_FLAG_INSERT_EXCLUSIVE: s.append("exclusive"); break;
 default: s.append("unknown/reserved"); break;
 }
    s.append(", ");

    s.append("volume mgmt: ");
 switch (flags & EFFECT_FLAG_VOLUME_MASK) {
 case EFFECT_FLAG_VOLUME_NONE: s.append("none"); break;
 case EFFECT_FLAG_VOLUME_CTRL: s.append("implements control"); break;
 case EFFECT_FLAG_VOLUME_IND: s.append("requires indication"); break;
 default: s.append("unknown/reserved"); break;
 }
    s.append(", ");

 uint32_t devind = flags & EFFECT_FLAG_DEVICE_MASK;
 if (devind) {
        s.append("device indication: ");
 switch (devind) {
 case EFFECT_FLAG_DEVICE_IND: s.append("requires updates"); break;
 default: s.append("unknown/reserved"); break;
 }
        s.append(", ");
 }

    s.append("input mode: ");
 switch (flags & EFFECT_FLAG_INPUT_MASK) {
 case EFFECT_FLAG_INPUT_DIRECT: s.append("direct"); break;
 case EFFECT_FLAG_INPUT_PROVIDER: s.append("provider"); break;
 case EFFECT_FLAG_INPUT_BOTH: s.append("direct+provider"); break;
 default: s.append("not set"); break;
 }
    s.append(", ");

    s.append("output mode: ");
 switch (flags & EFFECT_FLAG_OUTPUT_MASK) {
 case EFFECT_FLAG_OUTPUT_DIRECT: s.append("direct"); break;
 case EFFECT_FLAG_OUTPUT_PROVIDER: s.append("provider"); break;
 case EFFECT_FLAG_OUTPUT_BOTH: s.append("direct+provider"); break;
 default: s.append("not set"); break;
 }
    s.append(", ");

 uint32_t accel = flags & EFFECT_FLAG_HW_ACC_MASK;
 if (accel) {
        s.append("hardware acceleration: ");
 switch (accel) {
 case EFFECT_FLAG_HW_ACC_SIMPLE: s.append("non-tunneled"); break;
 case EFFECT_FLAG_HW_ACC_TUNNEL: s.append("tunneled"); break;
 default: s.append("unknown/reserved"); break;
 }
        s.append(", ");
 }

 uint32_t modeind = flags & EFFECT_FLAG_AUDIO_MODE_MASK;
 if (modeind) {
        s.append("mode indication: ");
 switch (modeind) {
 case EFFECT_FLAG_AUDIO_MODE_IND: s.append("required"); break;
 default: s.append("unknown/reserved"); break;
 }
        s.append(", ");
 }

 uint32_t srcind = flags & EFFECT_FLAG_AUDIO_SOURCE_MASK;
 if (srcind) {
        s.append("source indication: ");
 switch (srcind) {
 case EFFECT_FLAG_AUDIO_SOURCE_IND: s.append("required"); break;
 default: s.append("unknown/reserved"); break;
 }
        s.append(", ");
 }

 if (flags & EFFECT_FLAG_OFFLOAD_MASK) {
        s.append("offloadable, ");
 }

 int len = s.length();
 if (s.length() > 2) {
 (void) s.lockBuffer(len);
        s.unlockBuffer(len - 2);
 }
 return s;
}
