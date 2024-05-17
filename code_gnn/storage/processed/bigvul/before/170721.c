inline static const char *asString(IOMX::InternalOptionType i, const char *def = "??") {
 switch (i) {
 case IOMX::INTERNAL_OPTION_SUSPEND: return "SUSPEND";
 case IOMX::INTERNAL_OPTION_REPEAT_PREVIOUS_FRAME_DELAY:
 return "REPEAT_PREVIOUS_FRAME_DELAY";
 case IOMX::INTERNAL_OPTION_MAX_TIMESTAMP_GAP: return "MAX_TIMESTAMP_GAP";
 case IOMX::INTERNAL_OPTION_START_TIME: return "START_TIME";
 case IOMX::INTERNAL_OPTION_TIME_LAPSE: return "TIME_LAPSE";
 default: return def;
 }
}
