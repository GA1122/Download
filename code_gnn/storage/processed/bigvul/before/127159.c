DownloadPathReservationTrackerTest::DownloadPathReservationTrackerTest()
    : ui_thread_(BrowserThread::UI, &message_loop_),
      file_thread_(BrowserThread::FILE, &message_loop_) {
}
