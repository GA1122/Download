void WebsiteSettings::Init(Profile* profile,
                           const GURL& url,
                           const content::SSLStatus& ssl) {
  bool isChromeUINativeScheme = false;
#if defined(OS_ANDROID)
  isChromeUINativeScheme = url.SchemeIs(chrome::kChromeUINativeScheme);
#endif

  if (url.SchemeIs(content::kChromeUIScheme) ||
      url.SchemeIs(url::kAboutScheme) || isChromeUINativeScheme) {
    site_identity_status_ = SITE_IDENTITY_STATUS_INTERNAL_PAGE;
    site_identity_details_ =
        l10n_util::GetStringUTF16(IDS_PAGE_INFO_INTERNAL_PAGE);
    site_connection_status_ = SITE_CONNECTION_STATUS_INTERNAL_PAGE;
    return;
  }

  scoped_refptr<net::X509Certificate> cert;

  base::string16 subject_name(UTF8ToUTF16(url.host()));
  if (subject_name.empty()) {
    subject_name.assign(
        l10n_util::GetStringUTF16(IDS_PAGE_INFO_SECURITY_TAB_UNKNOWN_PARTY));
  }

  cert_id_ = ssl.cert_id;

  if (ssl.cert_id &&
      cert_store_->RetrieveCert(ssl.cert_id, &cert) &&
      (!net::IsCertStatusError(ssl.cert_status) ||
       net::IsCertStatusMinorError(ssl.cert_status))) {
#if defined(OS_CHROMEOS)
    policy::PolicyCertService* service =
        policy::PolicyCertServiceFactory::GetForProfile(profile);
    const bool used_policy_certs = service && service->UsedPolicyCertificates();
#else
    const bool used_policy_certs = false;
#endif
    if (used_policy_certs) {
      site_identity_status_ = SITE_IDENTITY_STATUS_ADMIN_PROVIDED_CERT;
      site_identity_details_ = l10n_util::GetStringFUTF16(
          IDS_CERT_POLICY_PROVIDED_CERT_MESSAGE, UTF8ToUTF16(url.host()));
    } else if (net::IsCertStatusMinorError(ssl.cert_status)) {
      site_identity_status_ = SITE_IDENTITY_STATUS_CERT_REVOCATION_UNKNOWN;
      base::string16 issuer_name(UTF8ToUTF16(cert->issuer().GetDisplayName()));
      if (issuer_name.empty()) {
        issuer_name.assign(l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_UNKNOWN_PARTY));
      }

      site_identity_details_.assign(l10n_util::GetStringFUTF16(
          GetSiteIdentityDetailsMessageByCTInfo(
              ssl.signed_certificate_timestamp_ids, false  ),
          issuer_name));

      site_identity_details_ += ASCIIToUTF16("\n\n");
      if (ssl.cert_status & net::CERT_STATUS_UNABLE_TO_CHECK_REVOCATION) {
        site_identity_details_ += l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_UNABLE_TO_CHECK_REVOCATION);
      } else if (ssl.cert_status & net::CERT_STATUS_NO_REVOCATION_MECHANISM) {
        site_identity_details_ += l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_NO_REVOCATION_MECHANISM);
      } else {
        NOTREACHED() << "Need to specify string for this warning";
      }
    } else {
      if (ssl.cert_status & net::CERT_STATUS_IS_EV) {
        site_identity_status_ = GetSiteIdentityStatusByCTInfo(
            ssl.signed_certificate_timestamp_ids, true);
        DCHECK(!cert->subject().organization_names.empty());
        organization_name_ = UTF8ToUTF16(cert->subject().organization_names[0]);
        DCHECK(!cert->subject().locality_name.empty());
        DCHECK(!cert->subject().country_name.empty());
        base::string16 locality;
        if (!cert->subject().state_or_province_name.empty()) {
          locality = l10n_util::GetStringFUTF16(
              IDS_PAGEINFO_ADDRESS,
              UTF8ToUTF16(cert->subject().locality_name),
              UTF8ToUTF16(cert->subject().state_or_province_name),
              UTF8ToUTF16(cert->subject().country_name));
        } else {
          locality = l10n_util::GetStringFUTF16(
              IDS_PAGEINFO_PARTIAL_ADDRESS,
              UTF8ToUTF16(cert->subject().locality_name),
              UTF8ToUTF16(cert->subject().country_name));
        }
        DCHECK(!cert->subject().organization_names.empty());
        site_identity_details_.assign(l10n_util::GetStringFUTF16(
            GetSiteIdentityDetailsMessageByCTInfo(
                ssl.signed_certificate_timestamp_ids, true  ),
            UTF8ToUTF16(cert->subject().organization_names[0]),
            locality,
            UTF8ToUTF16(cert->issuer().GetDisplayName())));
      } else {
        site_identity_status_ = GetSiteIdentityStatusByCTInfo(
            ssl.signed_certificate_timestamp_ids, false);
        base::string16 issuer_name(
            UTF8ToUTF16(cert->issuer().GetDisplayName()));
        if (issuer_name.empty()) {
          issuer_name.assign(l10n_util::GetStringUTF16(
              IDS_PAGE_INFO_SECURITY_TAB_UNKNOWN_PARTY));
        }

        site_identity_details_.assign(l10n_util::GetStringFUTF16(
            GetSiteIdentityDetailsMessageByCTInfo(
                ssl.signed_certificate_timestamp_ids, false  ),
            issuer_name));
      }
      static const int64_t kSHA1LastIssuanceDate = INT64_C(13096080000000000);
      if ((ssl.cert_status & net::CERT_STATUS_SHA1_SIGNATURE_PRESENT) &&
          cert->valid_expiry() >
              base::Time::FromInternalValue(kSHA1LastIssuanceDate)) {
        site_identity_status_ =
            SITE_IDENTITY_STATUS_DEPRECATED_SIGNATURE_ALGORITHM;
        site_identity_details_ +=
            UTF8ToUTF16("\n\n") +
            l10n_util::GetStringUTF16(
                IDS_PAGE_INFO_SECURITY_TAB_DEPRECATED_SIGNATURE_ALGORITHM);
      }
    }
  } else {
    site_identity_details_.assign(l10n_util::GetStringUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_INSECURE_IDENTITY));
    if (ssl.security_style == content::SECURITY_STYLE_UNAUTHENTICATED)
      site_identity_status_ = SITE_IDENTITY_STATUS_NO_CERT;
    else
      site_identity_status_ = SITE_IDENTITY_STATUS_ERROR;

    const base::string16 bullet = UTF8ToUTF16("\n • ");
    std::vector<SSLErrorInfo> errors;
    SSLErrorInfo::GetErrorsForCertStatus(ssl.cert_id, ssl.cert_status,
                                         url, &errors);
    for (size_t i = 0; i < errors.size(); ++i) {
      site_identity_details_ += bullet;
      site_identity_details_ += errors[i].short_description();
    }

    if (ssl.cert_status & net::CERT_STATUS_NON_UNIQUE_NAME) {
      site_identity_details_ += ASCIIToUTF16("\n\n");
      site_identity_details_ += l10n_util::GetStringUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_NON_UNIQUE_NAME);
    }
  }

  site_connection_status_ = SITE_CONNECTION_STATUS_UNKNOWN;

  if (ssl.security_style == content::SECURITY_STYLE_UNKNOWN) {
    DCHECK_EQ(ssl.security_bits, -1);
    site_connection_status_ = SITE_CONNECTION_STATUS_UNENCRYPTED;

    site_connection_details_.assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_NOT_ENCRYPTED_CONNECTION_TEXT,
        subject_name));
  } else if (ssl.security_style == content::SECURITY_STYLE_UNAUTHENTICATED) {
    DCHECK(!ssl.cert_id);
    site_connection_status_ = SITE_CONNECTION_STATUS_UNENCRYPTED;

    site_connection_details_.assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_NOT_ENCRYPTED_CONNECTION_TEXT,
        subject_name));
  } else if (ssl.security_bits < 0) {
    site_connection_status_ = SITE_CONNECTION_STATUS_ENCRYPTED_ERROR;
  } else if (ssl.security_bits == 0) {
    DCHECK_NE(ssl.security_style, content::SECURITY_STYLE_UNAUTHENTICATED);
    site_connection_status_ = SITE_CONNECTION_STATUS_ENCRYPTED_ERROR;
    site_connection_details_.assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_NOT_ENCRYPTED_CONNECTION_TEXT,
        subject_name));
  } else {
    site_connection_status_ = SITE_CONNECTION_STATUS_ENCRYPTED;

    if (net::SSLConnectionStatusToVersion(ssl.connection_status) >=
            net::SSL_CONNECTION_VERSION_TLS1_2 &&
        net::IsSecureTLSCipherSuite(
            net::SSLConnectionStatusToCipherSuite(ssl.connection_status))) {
      site_connection_details_.assign(l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_CONNECTION_TEXT,
          subject_name));
    } else {
      site_connection_details_.assign(l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_WEAK_ENCRYPTION_CONNECTION_TEXT,
          subject_name));
    }

    if (ssl.content_status) {
      bool ran_insecure_content =
          !!(ssl.content_status & content::SSLStatus::RAN_INSECURE_CONTENT);
      site_connection_status_ = ran_insecure_content ?
          SITE_CONNECTION_STATUS_ENCRYPTED_ERROR
          : SITE_CONNECTION_STATUS_MIXED_CONTENT;
      site_connection_details_.assign(l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_SENTENCE_LINK,
          site_connection_details_,
          l10n_util::GetStringUTF16(ran_insecure_content ?
              IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_INSECURE_CONTENT_ERROR :
              IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_INSECURE_CONTENT_WARNING)));
    }
  }

  uint16 cipher_suite =
      net::SSLConnectionStatusToCipherSuite(ssl.connection_status);
  if (ssl.security_bits > 0 && cipher_suite) {
    int ssl_version =
        net::SSLConnectionStatusToVersion(ssl.connection_status);
    const char* ssl_version_str;
    net::SSLVersionToString(&ssl_version_str, ssl_version);
    site_connection_details_ += ASCIIToUTF16("\n\n");
    site_connection_details_ += l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_SSL_VERSION,
        ASCIIToUTF16(ssl_version_str));

    bool no_renegotiation =
        (ssl.connection_status &
        net::SSL_CONNECTION_NO_RENEGOTIATION_EXTENSION) != 0;
    const char *key_exchange, *cipher, *mac;
    bool is_aead;
    net::SSLCipherSuiteToStrings(
        &key_exchange, &cipher, &mac, &is_aead, cipher_suite);

    site_connection_details_ += ASCIIToUTF16("\n\n");
    if (is_aead) {
      site_connection_details_ += l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTION_DETAILS_AEAD,
          ASCIIToUTF16(cipher), ASCIIToUTF16(key_exchange));
    } else {
      site_connection_details_ += l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTION_DETAILS,
          ASCIIToUTF16(cipher), ASCIIToUTF16(mac), ASCIIToUTF16(key_exchange));
    }

    if (ssl_version == net::SSL_CONNECTION_VERSION_SSL3 &&
        site_connection_status_ < SITE_CONNECTION_STATUS_MIXED_CONTENT) {
      site_connection_status_ = SITE_CONNECTION_STATUS_ENCRYPTED_ERROR;
    }

    const bool did_fallback =
        (ssl.connection_status & net::SSL_CONNECTION_VERSION_FALLBACK) != 0;
    if (did_fallback) {
      site_connection_details_ += ASCIIToUTF16("\n\n");
      site_connection_details_ += l10n_util::GetStringUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_FALLBACK_MESSAGE);
    }

    if (no_renegotiation) {
      site_connection_details_ += ASCIIToUTF16("\n\n");
      site_connection_details_ += l10n_util::GetStringUTF16(
          IDS_PAGE_INFO_SECURITY_TAB_RENEGOTIATION_MESSAGE);
    }
  }

  ChromeSSLHostStateDelegate* delegate =
      ChromeSSLHostStateDelegateFactory::GetForProfile(profile);
  DCHECK(delegate);
  show_ssl_decision_revoke_button_ = delegate->HasAllowException(url.host());

  WebsiteSettingsUI::TabId tab_id = WebsiteSettingsUI::TAB_ID_PERMISSIONS;
  if (site_connection_status_ == SITE_CONNECTION_STATUS_ENCRYPTED_ERROR ||
      site_connection_status_ == SITE_CONNECTION_STATUS_MIXED_CONTENT ||
      site_identity_status_ == SITE_IDENTITY_STATUS_ERROR ||
      site_identity_status_ == SITE_IDENTITY_STATUS_CERT_REVOCATION_UNKNOWN ||
      site_identity_status_ == SITE_IDENTITY_STATUS_ADMIN_PROVIDED_CERT ||
      site_identity_status_ ==
          SITE_IDENTITY_STATUS_DEPRECATED_SIGNATURE_ALGORITHM) {
    tab_id = WebsiteSettingsUI::TAB_ID_CONNECTION;
    RecordWebsiteSettingsAction(
      WEBSITE_SETTINGS_CONNECTION_TAB_SHOWN_IMMEDIATELY);
  }
  ui_->SetSelectedTab(tab_id);
}
