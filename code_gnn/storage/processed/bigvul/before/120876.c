void OneClickSigninSyncStarter::OnRegisteredForPolicy(
    scoped_ptr<policy::CloudPolicyClient> client) {
  SigninManager* signin = SigninManagerFactory::GetForProfile(profile_);
  if (!client.get()) {
    DVLOG(1) << "Policy registration failed";
    ConfirmAndSignin();
    return;
  }

  DCHECK(client->is_registered());
  DVLOG(1) << "Policy registration succeeded: dm_token=" << client->dm_token();

  DCHECK(!policy_client_);
  policy_client_.swap(client);

  EnsureBrowser();
  content::WebContents* web_contents =
      browser_->tab_strip_model()->GetActiveWebContents();
  if (!web_contents) {
    CancelSigninAndDelete();
    return;
  }
  chrome::ShowProfileSigninConfirmationDialog(
      browser_,
      web_contents,
      profile_,
      signin->GetUsernameForAuthInProgress(),
      new SigninDialogDelegate(weak_pointer_factory_.GetWeakPtr()));
}
