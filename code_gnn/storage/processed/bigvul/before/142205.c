void FileManagerBrowserTestBase::OnCommand(const std::string& name,
                                           const base::DictionaryValue& value,
                                           std::string* output) {
  base::ScopedAllowBlockingForTesting allow_blocking;

  if (name == "isInGuestMode") {
    if (IsGuestModeTest() || IsIncognitoModeTest()) {
      LOG(INFO) << GetTestCaseName() << " isInGuestMode: true";
      *output = "true";
    } else {
      ASSERT_EQ(NOT_IN_GUEST_MODE, GetGuestMode());
      *output = "false";
    }

    return;
  }

  if (name == "getDriveFsEnabled") {
    *output = IsDriveFsTest() ? "true" : "false";
    return;
  }

  if (name == "zipArchiverLoaded") {
    if (IsZipTest()) {
      LOG(INFO) << "Preloading zip archiver NaCl module";
      auto event = std::make_unique<extensions::Event>(
          extensions::events::FOR_TEST,
          extensions::api::test::OnMessage::kEventName,
          base::ListValue::From(base::JSONReader::ReadDeprecated(
              R"([{"data": "preloadZip", "lastMessage": false}])")),
          profile());
      extensions::EventRouter::Get(profile())->DispatchEventToExtension(
          kZipArchiverId, std::move(event));
    }
    return;
  }

  if (name == "getRootPaths") {
    const std::string downloads_path =
        base::FeatureList::IsEnabled(chromeos::features::kMyFilesVolume)
            ? "/Downloads"
            : "";
    const auto downloads_root =
        util::GetDownloadsMountPointName(profile()) + downloads_path;

    base::DictionaryValue dictionary;
    dictionary.SetString("downloads", "/" + downloads_root);
    dictionary.SetString("downloads_path", downloads_path);

    if (!profile()->IsGuestSession()) {
      auto* drive_integration_service =
          drive::DriveIntegrationServiceFactory::GetForProfile(profile());
      if (drive_integration_service->IsMounted()) {
        const auto drive_mount_name =
            base::FilePath(drive_integration_service->GetMountPointPath())
                .BaseName();
        dictionary.SetString(
            "drive", base::StrCat({"/", drive_mount_name.value(), "/root"}));
      }
      if (android_files_volume_) {
        dictionary.SetString("android_files",
                             "/" + util::GetAndroidFilesMountPointName());
      }
    }
    base::JSONWriter::Write(dictionary, output);
    return;
  }

  if (name == "getTestName") {
    *output = GetTestCaseName();
    return;
  }

  if (name == "getCwsWidgetContainerMockUrl") {
    const GURL url = embedded_test_server()->GetURL(
        "/chromeos/file_manager/cws_container_mock/index.html");
    std::string origin = url.GetOrigin().spec();
    if (*origin.rbegin() == '/')   
      origin.resize(origin.length() - 1);

    base::DictionaryValue dictionary;
    dictionary.SetString("url", url.spec());
    dictionary.SetString("origin", origin);

    base::JSONWriter::Write(dictionary, output);
    return;
  }

  if (name == "addEntries") {
    AddEntriesMessage message;
    ASSERT_TRUE(AddEntriesMessage::ConvertJSONValue(value, &message));

    for (size_t i = 0; i < message.entries.size(); ++i) {
      switch (message.volume) {
        case AddEntriesMessage::LOCAL_VOLUME:
          local_volume_->CreateEntry(*message.entries[i]);
          break;
        case AddEntriesMessage::CROSTINI_VOLUME:
          CHECK(crostini_volume_);
          ASSERT_TRUE(crostini_volume_->Initialize(profile()));
          crostini_volume_->CreateEntry(*message.entries[i]);
          break;
        case AddEntriesMessage::DRIVE_VOLUME:
          if (drive_volume_) {
            drive_volume_->CreateEntry(*message.entries[i]);
          } else if (!IsGuestModeTest()) {
            LOG(FATAL) << "Add entry: but no Drive volume.";
          }
          break;
        case AddEntriesMessage::USB_VOLUME:
          if (usb_volume_) {
            usb_volume_->CreateEntry(*message.entries[i]);
          } else {
            LOG(FATAL) << "Add entry: but no USB volume.";
          }
          break;
        case AddEntriesMessage::ANDROID_FILES_VOLUME:
          if (android_files_volume_) {
            android_files_volume_->CreateEntry(*message.entries[i]);
          } else {
            LOG(FATAL) << "Add entry: but no Android files volume.";
          }
          break;
        case AddEntriesMessage::DOCUMENTS_PROVIDER_VOLUME:
          if (documents_provider_volume_) {
            documents_provider_volume_->CreateEntry(*message.entries[i]);
          } else {
            LOG(FATAL) << "Add entry: but no DocumentsProvider volume.";
          }
          break;
      }
    }

    return;
  }

  if (name == "mountFakeUsb" || name == "mountFakeUsbEmpty" ||
      name == "mountFakeUsbDcim") {
    usb_volume_ = std::make_unique<FakeTestVolume>(
        "fake-usb", VOLUME_TYPE_REMOVABLE_DISK_PARTITION,
        chromeos::DEVICE_TYPE_USB);

    if (name == "mountFakeUsb")
      ASSERT_TRUE(usb_volume_->PrepareTestEntries(profile()));
    else if (name == "mountFakeUsbDcim")
      ASSERT_TRUE(usb_volume_->PrepareDcimTestEntries(profile()));

    ASSERT_TRUE(usb_volume_->Mount(profile()));
    return;
  }

  if (name == "unmountUsb") {
    DCHECK(usb_volume_);
    usb_volume_->Unmount(profile());
  }

  if (name == "mountUsbWithPartitions") {
    constexpr char kDevicePath[] =
        "sys/devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2.2/1-2.2:1.0/host0/"
        "target0:0:0/0:0:0:0";
    const base::FilePath device_path(kDevicePath);

    partition_1_ = std::make_unique<RemovableTestVolume>(
        "partition-1", VOLUME_TYPE_REMOVABLE_DISK_PARTITION,
        chromeos::DEVICE_TYPE_USB, device_path, "Drive Label");
    partition_2_ = std::make_unique<RemovableTestVolume>(
        "partition-2", VOLUME_TYPE_REMOVABLE_DISK_PARTITION,
        chromeos::DEVICE_TYPE_USB, device_path, "Drive Label");

    ASSERT_TRUE(partition_1_->PreparePartitionTestEntries(profile()));
    ASSERT_TRUE(partition_2_->PreparePartitionTestEntries(profile()));

    ASSERT_TRUE(partition_1_->Mount(profile()));
    ASSERT_TRUE(partition_2_->Mount(profile()));
    return;
  }

  if (name == "unmountPartitions") {
    DCHECK(partition_1_);
    DCHECK(partition_2_);
    partition_1_->Unmount(profile());
    partition_2_->Unmount(profile());
    return;
  }

  if (name == "mountFakeMtp" || name == "mountFakeMtpEmpty") {
    mtp_volume_ = std::make_unique<FakeTestVolume>(
        "fake-mtp", VOLUME_TYPE_MTP, chromeos::DEVICE_TYPE_UNKNOWN);

    if (name == "mountFakeMtp")
      ASSERT_TRUE(mtp_volume_->PrepareTestEntries(profile()));

    ASSERT_TRUE(mtp_volume_->Mount(profile()));
    return;
  }

  if (name == "mountDrive") {
    ASSERT_TRUE(drive_volume_->Mount(profile()));
    return;
  }

  if (name == "unmountDrive") {
    drive_volume_->Unmount();
    return;
  }

  if (name == "mountDownloads") {
    ASSERT_TRUE(local_volume_->Mount(profile()));
    return;
  }

  if (name == "unmountDownloads") {
    local_volume_->Unmount(profile());
    return;
  }

  if (name == "mountMediaView") {
    CHECK(arc::IsArcAvailable())
        << "ARC required for mounting media view volumes";

    media_view_images_ = std::make_unique<MediaViewTestVolume>(
        arc_file_system_instance_.get(),
        "com.android.providers.media.documents", arc::kImagesRootDocumentId);
    media_view_videos_ = std::make_unique<MediaViewTestVolume>(
        arc_file_system_instance_.get(),
        "com.android.providers.media.documents", arc::kVideosRootDocumentId);
    media_view_audio_ = std::make_unique<MediaViewTestVolume>(
        arc_file_system_instance_.get(),
        "com.android.providers.media.documents", arc::kAudioRootDocumentId);

    ASSERT_TRUE(media_view_images_->Mount(profile()));
    ASSERT_TRUE(media_view_videos_->Mount(profile()));
    ASSERT_TRUE(media_view_audio_->Mount(profile()));
    return;
  }

  if (name == "mountPlayFiles") {
    DCHECK(android_files_volume_);
    android_files_volume_->Mount(profile());
    return;
  }

  if (name == "unmountPlayFiles") {
    DCHECK(android_files_volume_);
    android_files_volume_->Unmount(profile());
    return;
  }

  if (name == "setDriveEnabled") {
    bool enabled;
    ASSERT_TRUE(value.GetBoolean("enabled", &enabled));
    profile()->GetPrefs()->SetBoolean(drive::prefs::kDisableDrive, !enabled);
    return;
  }

  if (name == "useCellularNetwork") {
    net::NetworkChangeNotifier::NotifyObserversOfMaxBandwidthChangeForTests(
        net::NetworkChangeNotifier::GetMaxBandwidthMbpsForConnectionSubtype(
            net::NetworkChangeNotifier::SUBTYPE_HSPA),
        net::NetworkChangeNotifier::CONNECTION_3G);
    return;
  }

  if (name == "clickNotificationButton") {
    std::string extension_id;
    std::string notification_id;
    ASSERT_TRUE(value.GetString("extensionId", &extension_id));
    ASSERT_TRUE(value.GetString("notificationId", &notification_id));

    const std::string delegate_id = extension_id + "-" + notification_id;
    base::Optional<message_center::Notification> notification =
        display_service_->GetNotification(delegate_id);
    EXPECT_TRUE(notification);

    int index;
    ASSERT_TRUE(value.GetInteger("index", &index));
    display_service_->SimulateClick(NotificationHandler::Type::EXTENSION,
                                    delegate_id, index, base::nullopt);
    return;
  }

  if (name == "launchProviderExtension") {
    std::string manifest;
    ASSERT_TRUE(value.GetString("manifest", &manifest));
    LaunchExtension(base::FilePath(FILE_PATH_LITERAL(
                        "ui/file_manager/integration_tests/testing_provider")),
                    manifest.c_str());
    return;
  }

  if (name == "dispatchNativeMediaKey") {
    ui::KeyEvent key_event(ui::ET_KEY_PRESSED, ui::VKEY_MEDIA_PLAY_PAUSE, 0);

    const auto& app_windows =
        extensions::AppWindowRegistry::Get(profile())->app_windows();
    ASSERT_FALSE(app_windows.empty());
    app_windows.front()->GetNativeWindow()->GetHost()->DispatchKeyEventPostIME(
        &key_event, base::NullCallback());
    *output = "mediaKeyDispatched";
    return;
  }

  if (name == "dispatchTabKey") {
    ui::KeyEvent key_event(ui::ET_KEY_PRESSED, ui::VKEY_TAB, 0);

    const auto& app_windows =
        extensions::AppWindowRegistry::Get(profile())->app_windows();
    aura::WindowTreeHost* host = nullptr;
    if (app_windows.empty()) {
      ASSERT_TRUE(select_factory_);
      views::Widget* widget = select_factory_->GetLastWidget();
      ASSERT_TRUE(widget);
      host = widget->GetNativeWindow()->GetHost();
    } else {
      host = app_windows.front()->GetNativeWindow()->GetHost();
    }
    ASSERT_TRUE(host);
    host->DispatchKeyEventPostIME(&key_event, base::NullCallback());
    *output = "tabKeyDispatched";
    return;
  }

  if (name == "getAppWindowId") {
    std::string window_url;
    ASSERT_TRUE(value.GetString("windowUrl", &window_url));

    const auto& app_windows =
        extensions::AppWindowRegistry::Get(profile())->app_windows();
    ASSERT_FALSE(app_windows.empty());
    *output = "none";
    for (auto* window : app_windows) {
      if (!window->web_contents())
        continue;

      if (window->web_contents()->GetLastCommittedURL() == window_url) {
        *output = base::NumberToString(window->session_id().id());
        break;
      }
    }
    return;
  }

  if (name == "countAppWindows") {
    std::string app_id;
    ASSERT_TRUE(value.GetString("appId", &app_id));

    const auto& app_windows =
        extensions::AppWindowRegistry::Get(profile())->app_windows();
    ASSERT_FALSE(app_windows.empty());
    int window_count = 0;
    for (auto* window : app_windows) {
      if (window->extension_id() == app_id)
        window_count++;
    }
    *output = base::NumberToString(window_count);
    return;
  }

  if (name == "runJsInAppWindow") {
    std::string window_id_str;
    ASSERT_TRUE(value.GetString("windowId", &window_id_str));
    int window_id = 0;
    ASSERT_TRUE(base::StringToInt(window_id_str, &window_id));
    std::string script;
    ASSERT_TRUE(value.GetString("script", &script));

    const auto& app_windows =
        extensions::AppWindowRegistry::Get(profile())->app_windows();
    ASSERT_FALSE(app_windows.empty());
    for (auto* window : app_windows) {
      CHECK(window);
      if (window->session_id().id() != window_id) {
        continue;
      }

      if (!window->web_contents())
        break;

      CHECK(window->web_contents()->GetMainFrame());
      window->web_contents()->GetMainFrame()->ExecuteJavaScriptForTests(
          base::UTF8ToUTF16(script), base::NullCallback());

      break;
    }
    return;
  }

  if (name == "enableTabletMode") {
    ::test::SetAndWaitForTabletMode(true);
    *output = "tabletModeEnabled";
    return;
  }

  if (name == "runSelectFileDialog") {
    browser()->OpenFile();
    content::TestNavigationObserver observer(
        browser()->tab_strip_model()->GetActiveWebContents(), 1);
    observer.Wait();
    *output = observer.last_navigation_url().spec();
    return;
  }

  if (name == "isSmbEnabled") {
    *output = IsNativeSmbTest() ? "true" : "false";
    return;
  }

  if (name == "runLauncherSearch") {
    app_list::LauncherSearchProvider search_provider(profile());
    base::string16 query;
    ASSERT_TRUE(value.GetString("query", &query));

    search_provider.Start(query);
    base::RunLoop run_loop;
    search_provider.set_result_changed_callback(run_loop.QuitClosure());
    run_loop.Run();

    std::vector<base::Value> names;
    for (auto& result : search_provider.results()) {
      names.emplace_back(result->title());
    }
    std::sort(names.begin(), names.end());
    base::JSONWriter::Write(base::Value(std::move(names)), output);
    return;
  }

  if (name == "blockFileTaskRunner") {
    BlockFileTaskRunner(profile());
    return;
  }

  if (name == "unblockFileTaskRunner") {
    UnblockFileTaskRunner();
    return;
  }

  if (name == "expectFileTask") {
    ExpectFileTasksMessage message;
    ASSERT_TRUE(ExpectFileTasksMessage::ConvertJSONValue(value, &message));
    if (!file_tasks_observer_) {
      return;
    }
    for (const auto& file_name : message.file_names) {
      EXPECT_CALL(
          *file_tasks_observer_,
          OnFilesOpenedImpl(testing::HasSubstr(*file_name), message.open_type));
    }
    return;
  }

  FAIL() << "Unknown test message: " << name;
}
