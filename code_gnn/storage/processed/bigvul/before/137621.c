 void DownloadController::OnDownloadUpdated(DownloadItem* item) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (item->IsDangerous() && (item->GetState() != DownloadItem::CANCELLED))
    OnDangerousDownload(item);

  JNIEnv* env = base::android::AttachCurrentThread();
  ScopedJavaLocalRef<jstring> jguid =
      ConvertUTF8ToJavaString(env, item->GetGuid());
  ScopedJavaLocalRef<jstring> jurl =
      ConvertUTF8ToJavaString(env, item->GetURL().spec());
  ScopedJavaLocalRef<jstring> jmime_type =
      ConvertUTF8ToJavaString(env, item->GetMimeType());
  ScopedJavaLocalRef<jstring> jpath =
      ConvertUTF8ToJavaString(env, item->GetTargetFilePath().value());
  ScopedJavaLocalRef<jstring> jfilename = ConvertUTF8ToJavaString(
      env, item->GetTargetFilePath().BaseName().value());
  ScopedJavaLocalRef<jstring> joriginal_url =
      ConvertUTF8ToJavaString(env, item->GetOriginalUrl().spec());
  ScopedJavaLocalRef<jstring> jreferrer_url =
      ConvertUTF8ToJavaString(env, item->GetReferrerUrl().spec());

  ui::PageTransition base_transition =
      ui::PageTransitionStripQualifier(item->GetTransitionType());
  bool user_initiated =
      item->GetTransitionType() & ui::PAGE_TRANSITION_FROM_ADDRESS_BAR ||
      base_transition == ui::PAGE_TRANSITION_TYPED ||
      base_transition == ui::PAGE_TRANSITION_AUTO_BOOKMARK ||
      base_transition == ui::PAGE_TRANSITION_GENERATED ||
      base_transition == ui::PAGE_TRANSITION_RELOAD ||
      base_transition == ui::PAGE_TRANSITION_KEYWORD;
  bool hasUserGesture = item->HasUserGesture() || user_initiated;
  switch (item->GetState()) {
    case DownloadItem::IN_PROGRESS: {
      base::TimeDelta time_delta;
      item->TimeRemaining(&time_delta);
      Java_DownloadController_onDownloadUpdated(
          env, GetJavaObject()->Controller(env), jurl, jmime_type, jfilename,
          jpath, item->GetReceivedBytes(), jguid, item->PercentComplete(),
          time_delta.InMilliseconds(), hasUserGesture, item->IsPaused(),
          item->GetBrowserContext()->IsOffTheRecord());
      break;
    }
    case DownloadItem::COMPLETE:
      item->RemoveObserver(this);

      Java_DownloadController_onDownloadCompleted(
          env, GetJavaObject()->Controller(env), jurl, jmime_type, jfilename,
          jpath, item->GetReceivedBytes(), jguid, joriginal_url, jreferrer_url,
          hasUserGesture);
      DownloadController::RecordDownloadCancelReason(
             DownloadController::CANCEL_REASON_NOT_CANCELED);
      break;
    case DownloadItem::CANCELLED:
      Java_DownloadController_onDownloadCancelled(
          env, GetJavaObject()->Controller(env), jguid);
      break;
    case DownloadItem::INTERRUPTED:
      Java_DownloadController_onDownloadInterrupted(
          env, GetJavaObject()->Controller(env), jurl, jmime_type, jfilename,
          jpath, item->GetReceivedBytes(), jguid, item->CanResume(),
          IsInterruptedDownloadAutoResumable(item),
          item->GetBrowserContext()->IsOffTheRecord());
      item->RemoveObserver(this);
      break;
    case DownloadItem::MAX_DOWNLOAD_STATE:
      NOTREACHED();
  }
}
