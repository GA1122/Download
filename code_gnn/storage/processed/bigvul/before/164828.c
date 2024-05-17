  void DownloadFilesCheckErrorsLoopBody(const DownloadInfo& download_info,
                                        size_t i) {
    SCOPED_TRACE(testing::Message()
                 << " " << __FUNCTION__ << "()"
                 << " index = " << i << " starting_url = '"
                 << download_info.starting_url << "'"
                 << " download_url = '" << download_info.expected_download_url
                 << "'"
                 << " method = "
                 << ((download_info.download_method == DOWNLOAD_DIRECT)
                         ? "DOWNLOAD_DIRECT"
                         : "DOWNLOAD_NAVIGATE")
                 << " show_item = " << download_info.show_download_item
                 << " reason = "
                 << DownloadInterruptReasonToString(download_info.reason));

    std::vector<DownloadItem*> download_items;
    GetDownloads(browser(), &download_items);
    size_t downloads_expected = download_items.size();

    GURL base_url = embedded_test_server()->GetURL("/downloads/");
    GURL starting_url = base_url.Resolve(download_info.starting_url);
    GURL download_url = base_url.Resolve(download_info.expected_download_url);
    ASSERT_TRUE(starting_url.is_valid());
    ASSERT_TRUE(download_url.is_valid());

    DownloadManager* download_manager = DownloadManagerForBrowser(browser());
    WebContents* web_contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    ASSERT_TRUE(web_contents);

    std::unique_ptr<content::DownloadTestObserver> observer;
    if (download_info.reason == download::DOWNLOAD_INTERRUPT_REASON_NONE) {
      observer.reset(new content::DownloadTestObserverTerminal(
          download_manager, 1,
          content::DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL));
    } else {
      observer.reset(new content::DownloadTestObserverInterrupted(
          download_manager, 1,
          content::DownloadTestObserver::ON_DANGEROUS_DOWNLOAD_FAIL));
    }

    if (download_info.download_method == DOWNLOAD_DIRECT) {
      scoped_refptr<content::DownloadTestItemCreationObserver>
          creation_observer(new content::DownloadTestItemCreationObserver);

      std::unique_ptr<DownloadUrlParameters> params(
          content::DownloadRequestUtils::CreateDownloadForWebContentsMainFrame(
              web_contents, starting_url, TRAFFIC_ANNOTATION_FOR_TESTS));
      params->set_callback(creation_observer->callback());
      DownloadManagerForBrowser(browser())->DownloadUrl(std::move(params));

      creation_observer->WaitForDownloadItemCreation();

      EXPECT_NE(download::DownloadItem::kInvalidId,
                creation_observer->download_id());
    } else {
      ui_test_utils::NavigateToURLBlockUntilNavigationsComplete(
          browser(), starting_url, 1);
    }

    if (download_info.show_download_item) {
      downloads_expected++;
      observer->WaitForFinished();
      DownloadItem::DownloadState final_state =
          (download_info.reason == download::DOWNLOAD_INTERRUPT_REASON_NONE)
              ? DownloadItem::COMPLETE
              : DownloadItem::INTERRUPTED;
      EXPECT_EQ(1u, observer->NumDownloadsSeenInState(final_state));
    }

    content::RunAllTasksUntilIdle();

    download_items.clear();
    GetDownloads(browser(), &download_items);
    ASSERT_EQ(downloads_expected, download_items.size());

    if (download_info.show_download_item) {
      DownloadItem* item = download_items[0];
      for (size_t d = 1; d < downloads_expected; ++d) {
        if (download_items[d]->GetStartTime() > item->GetStartTime())
          item = download_items[d];
      }

      EXPECT_EQ(download_url, item->GetURL());
      EXPECT_EQ(download_info.reason, item->GetLastReason());

      if (item->GetState() == download::DownloadItem::COMPLETE) {
        base::ScopedAllowBlockingForTesting allow_blocking;
        base::FilePath destination_folder = GetDownloadDirectory(browser());
        base::FilePath my_downloaded_file = item->GetTargetFilePath();
        EXPECT_TRUE(base::PathExists(my_downloaded_file));
        EXPECT_TRUE(base::DeleteFile(my_downloaded_file, false));

        EXPECT_EQ(download_info.should_redirect_to_documents ?
                      std::string::npos :
                      0u,
                  my_downloaded_file.value().find(destination_folder.value()));
        if (download_info.should_redirect_to_documents) {
          base::FilePath my_docs_folder;
          EXPECT_TRUE(base::PathService::Get(chrome::DIR_USER_DOCUMENTS,
                                             &my_docs_folder));
          EXPECT_EQ(0u,
                    my_downloaded_file.value().find(my_docs_folder.value()));
        }
      }
    }
  }
