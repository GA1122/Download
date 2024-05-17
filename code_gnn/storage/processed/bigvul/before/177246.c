static inline bool isOMXError(int32_t err) {
 return (ERROR_CODEC_MIN <= err && err <= ERROR_CODEC_MAX);
}
