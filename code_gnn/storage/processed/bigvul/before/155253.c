void BuildHandlerArgs(CrashReporterClient* crash_reporter_client,
                      base::FilePath* database_path,
                      base::FilePath* metrics_path,
                      std::string* url,
                      std::map<std::string, std::string>* process_annotations,
                      std::vector<std::string>* arguments) {
  crash_reporter_client->GetCrashDumpLocation(database_path);
  crash_reporter_client->GetCrashMetricsLocation(metrics_path);

#if defined(GOOGLE_CHROME_BUILD) && defined(OFFICIAL_BUILD) && \
    !defined(OS_ANDROID)
  *url = "https://clients2.google.com/cr/report";
#else
  *url = std::string();
#endif

  std::string product_name;
  std::string product_version;
  std::string channel;
  crash_reporter_client->GetProductNameAndVersion(&product_name,
                                                  &product_version, &channel);
  (*process_annotations)["prod"] = product_name;
  (*process_annotations)["ver"] = product_version;

#if defined(OS_ANDROID)
  SetBuildInfoAnnotations(process_annotations);
#endif   

#if defined(GOOGLE_CHROME_BUILD)
  const bool allow_empty_channel = true;
#else
  const bool allow_empty_channel = false;
#endif
  if (allow_empty_channel || !channel.empty()) {
    (*process_annotations)["channel"] = channel;
  }

#if defined(OS_ANDROID)
  (*process_annotations)["plat"] = std::string("Android");
#else
  (*process_annotations)["plat"] = std::string("Linux");
#endif

  if (crash_reporter_client->ShouldMonitorCrashHandlerExpensively()) {
    arguments->push_back("--monitor-self");
  }

  arguments->push_back("--monitor-self-annotation=ptype=crashpad-handler");
}
