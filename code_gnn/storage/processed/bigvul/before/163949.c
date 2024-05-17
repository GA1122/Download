PaymentHandlerWebFlowViewController::PaymentHandlerWebFlowViewController(
    PaymentRequestSpec* spec,
    PaymentRequestState* state,
    PaymentRequestDialogView* dialog,
    content::WebContents* payment_request_web_contents,
    Profile* profile,
    GURL target,
    PaymentHandlerOpenWindowCallback first_navigation_complete_callback)
    : PaymentRequestSheetController(spec, state, dialog),
      log_(payment_request_web_contents),
      profile_(profile),
      target_(target),
      show_progress_bar_(false),
      progress_bar_(
          std::make_unique<views::ProgressBar>( 2)),
      separator_(std::make_unique<views::Separator>()),
      first_navigation_complete_callback_(
          std::move(first_navigation_complete_callback)),
      https_prefix_(base::UTF8ToUTF16(url::kHttpsScheme) +
                    base::UTF8ToUTF16(url::kStandardSchemeSeparator)),
      dialog_manager_delegate_(
          static_cast<web_modal::WebContentsModalDialogManagerDelegate*>(
              chrome::FindBrowserWithWebContents(payment_request_web_contents))
              ->GetWebContentsModalDialogHost()) {
  progress_bar_->set_owned_by_client();
  progress_bar_->set_foreground_color(gfx::kGoogleBlue500);
  progress_bar_->set_background_color(SK_ColorTRANSPARENT);
  separator_->set_owned_by_client();
  separator_->SetColor(separator_->GetNativeTheme()->GetSystemColor(
       ui::NativeTheme::kColorId_SeparatorColor));
 }
