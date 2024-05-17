void SupervisedUserService::OnSupervisedUserIdChanged() {
  SetActive(ProfileIsSupervised());
}
