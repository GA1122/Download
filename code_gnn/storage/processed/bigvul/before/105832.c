STDMETHODIMP UrlmonUrlRequest::OnProgress(ULONG progress, ULONG max_progress,
    ULONG status_code, LPCWSTR status_text) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());

  if (status_.get_state() != Status::WORKING)
    return S_OK;

  if (pending_ && status_code != BINDSTATUS_REDIRECTING)
    return S_OK;

  if (!delegate_) {
    DVLOG(1) << "Invalid delegate";
    return S_OK;
  }

  switch (status_code) {
    case BINDSTATUS_CONNECTING: {
      if (status_text) {
        socket_address_.set_host(WideToUTF8(status_text));
      }
      break;
    }

    case BINDSTATUS_REDIRECTING: {
      base::win::ScopedComPtr<BindContextInfo> info;
      BindContextInfo::FromBindContext(bind_context_, info.Receive());
      DCHECK(info);
      GURL previously_redirected(info ? info->GetUrl() : std::wstring());
      if (GURL(status_text) != previously_redirected) {
        DVLOG(1) << __FUNCTION__ << me() << "redirect from " << url()
                 << " to " << status_text;
        int http_code = GetHttpResponseStatusFromBinding(binding_);
        status_.SetRedirected(http_code, WideToUTF8(status_text));
        binding_->Abort();
        return E_ABORT;
      }
      break;
    }

    case BINDSTATUS_COOKIE_SENT:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_READ);
      break;

    case BINDSTATUS_COOKIE_SUPPRESSED:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_SUPPRESS);
      break;

    case BINDSTATUS_COOKIE_STATE_ACCEPT:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_ACCEPT);
      break;

    case BINDSTATUS_COOKIE_STATE_REJECT:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_REJECT);
      break;

    case BINDSTATUS_COOKIE_STATE_LEASH:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_LEASH);
      break;

    case BINDSTATUS_COOKIE_STATE_DOWNGRADE:
      delegate_->AddPrivacyDataForUrl(url(), "", COOKIEACTION_DOWNGRADE);
      break;

    case BINDSTATUS_COOKIE_STATE_UNKNOWN:
      NOTREACHED() << L"Unknown cookie state received";
      break;

    default:
      DVLOG(1) << __FUNCTION__ << me()
               << base::StringPrintf(L"code: %i status: %ls", status_code,
                                     status_text);
      break;
  }

  return S_OK;
}
