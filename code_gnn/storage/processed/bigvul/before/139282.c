  void RunGetUserMediaAndCollectMeasures(const int time_to_sample_secs,
                                         const std::string& measure_filter,
                                         const std::string& graph_name) {
    ASSERT_TRUE(embedded_test_server()->InitializeAndWaitUntilReady());

    GURL url(embedded_test_server()->GetURL("/media/getusermedia.html"));
    NavigateToURL(shell(), url);

    DCHECK(time_to_sample_secs);
    ExecuteJavascriptAndWaitForOk(
        base::StringPrintf("%s({video: true}, 'myStreamName');",
                           kGetUserMediaAndGetStreamUp));

    StartTracing();

    ExecuteJavascriptAndWaitForOk(
        base::StringPrintf("waitAndStopVideoTrack(window['myStreamName'], %d);",
                           time_to_sample_secs));

    StopTracing();

    scoped_ptr<TraceAnalyzer> analyzer(CreateTraceAnalyzer());
    analyzer->AssociateBeginEndEvents();
    trace_analyzer::TraceEventVector events;
    DCHECK(measure_filter.size());
    analyzer->FindEvents(
        Query::EventNameIs(measure_filter),
        &events);
    ASSERT_GT(events.size(), 0u)
        << "Could not collect any samples during test, this is bad";

    std::string duration_us;
    std::string interarrival_us;
    for (size_t i = 0; i != events.size(); ++i) {
      duration_us.append(
          base::StringPrintf("%d,", static_cast<int>(events[i]->duration)));
    }

    for (size_t i = 1; i < events.size(); ++i) {
      interarrival_us.append(base::StringPrintf("%d,",
          static_cast<int>((events[i]->timestamp - events[i - 1]->timestamp) /
                           base::Time::kNanosecondsPerMicrosecond)));
    }

    perf_test::PrintResultList(
        graph_name, "", "sample_duration", duration_us, "us", true);

    perf_test::PrintResultList(
        graph_name, "", "interarrival_time", interarrival_us, "us", true);
  }
