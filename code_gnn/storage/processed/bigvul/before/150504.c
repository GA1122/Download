 void SafeBrowsingPrivateEventRouter::SetCloudPolicyClientForTesting(
     std::unique_ptr<policy::CloudPolicyClient> client) {
   DCHECK_EQ(nullptr, client_.get());
  client_ = std::move(client);
}
