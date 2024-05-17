static inline bool isEngBuild() {
 static const std::string sBuildType = android::base::GetProperty("ro.build.type", "user");
 return sBuildType == "eng";
}
