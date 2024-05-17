  TestSafeBrowsingBlockingPage(SafeBrowsingService* service,
                               TabContents* tab_contents,
                               const UnsafeResourceList& unsafe_resources)
      : SafeBrowsingBlockingPage(service, tab_contents, unsafe_resources) {
    malware_details_proceed_delay_ms_ = 100;
    wait_for_delete_ = false;
  }
