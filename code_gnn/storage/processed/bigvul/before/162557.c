void Resource::OnMemoryDump(WebMemoryDumpLevelOfDetail level_of_detail,
                            WebProcessMemoryDump* memory_dump) const {
  static const size_t kMaxURLReportLength = 128;
  static const int kMaxResourceClientToShowInMemoryInfra = 10;

  const String dump_name = GetMemoryDumpName();
  WebMemoryAllocatorDump* dump =
      memory_dump->CreateMemoryAllocatorDump(dump_name);
  dump->AddScalar("encoded_size", "bytes", encoded_size_memory_usage_);
  if (HasClientsOrObservers())
    dump->AddScalar("live_size", "bytes", encoded_size_memory_usage_);
  else
    dump->AddScalar("dead_size", "bytes", encoded_size_memory_usage_);

  if (data_)
    data_->OnMemoryDump(dump_name, memory_dump);

  if (level_of_detail == WebMemoryDumpLevelOfDetail::kDetailed) {
    String url_to_report = Url().GetString();
    if (url_to_report.length() > kMaxURLReportLength) {
      url_to_report.Truncate(kMaxURLReportLength);
      url_to_report = url_to_report + "...";
    }
    dump->AddString("url", "", url_to_report);

    dump->AddString("reason_not_deletable", "", ReasonNotDeletable());

    Vector<String> client_names;
    ResourceClientWalker<ResourceClient> walker(clients_);
    while (ResourceClient* client = walker.Next())
      client_names.push_back(client->DebugName());
    ResourceClientWalker<ResourceClient> walker2(clients_awaiting_callback_);
    while (ResourceClient* client = walker2.Next())
      client_names.push_back("(awaiting) " + client->DebugName());
    ResourceClientWalker<ResourceClient> walker3(finished_clients_);
    while (ResourceClient* client = walker3.Next())
      client_names.push_back("(finished) " + client->DebugName());
    std::sort(client_names.begin(), client_names.end(),
              WTF::CodePointCompareLessThan);

    StringBuilder builder;
    for (size_t i = 0;
         i < client_names.size() && i < kMaxResourceClientToShowInMemoryInfra;
         ++i) {
      if (i > 0)
        builder.Append(" / ");
      builder.Append(client_names[i]);
    }
    if (client_names.size() > kMaxResourceClientToShowInMemoryInfra) {
      builder.Append(" / and ");
      builder.AppendNumber(client_names.size() -
                           kMaxResourceClientToShowInMemoryInfra);
      builder.Append(" more");
    }
    dump->AddString("ResourceClient", "", builder.ToString());
  }

  const String overhead_name = dump_name + "/metadata";
  WebMemoryAllocatorDump* overhead_dump =
      memory_dump->CreateMemoryAllocatorDump(overhead_name);
  overhead_dump->AddScalar("size", "bytes", OverheadSize());
  memory_dump->AddSuballocation(
      overhead_dump->Guid(), String(WTF::Partitions::kAllocatedObjectPoolName));
}
