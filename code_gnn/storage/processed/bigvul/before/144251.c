void GetMultiProfilePolicy(const user_manager::User* user,
                           bool* out_is_allowed,
                           ash::mojom::MultiProfileUserBehavior* out_policy) {
  const std::string& user_id = user->GetAccountId().GetUserEmail();
  MultiProfileUserController* multi_profile_user_controller =
      ChromeUserManager::Get()->GetMultiProfileUserController();
  MultiProfileUserController::UserAllowedInSessionReason is_user_allowed_reason;
  *out_is_allowed = multi_profile_user_controller->IsUserAllowedInSession(
      user_id, &is_user_allowed_reason);

  std::string policy;
  if (is_user_allowed_reason ==
      MultiProfileUserController::NOT_ALLOWED_OWNER_AS_SECONDARY) {
    policy = MultiProfileUserController::kBehaviorOwnerPrimaryOnly;
  } else {
    policy = multi_profile_user_controller->GetCachedValue(user_id);
  }
  *out_policy = MultiProfileUserController::UserBehaviorStringToEnum(policy);
}
