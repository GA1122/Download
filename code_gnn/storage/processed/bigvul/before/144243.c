ash::mojom::UserAvatarPtr UserSelectionScreen::BuildMojoUserAvatarForUser(
    const user_manager::User* user) {
  auto avatar = ash::mojom::UserAvatar::New();
  if (!user->GetImage().isNull()) {
    avatar->image = user->GetImage();
  } else {
    avatar->image = *ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
        IDR_LOGIN_DEFAULT_USER);
  }

  auto load_image_from_resource = [&](int resource_id) {
    auto& rb = ui::ResourceBundle::GetSharedInstance();
    base::StringPiece avatar_data =
        rb.GetRawDataResourceForScale(resource_id, rb.GetMaxScaleFactor());
    avatar->bytes.assign(avatar_data.begin(), avatar_data.end());
  };
  if (user->has_image_bytes()) {
    avatar->bytes.assign(
        user->image_bytes()->front(),
        user->image_bytes()->front() + user->image_bytes()->size());
  } else if (user->HasDefaultImage()) {
    int resource_id = chromeos::default_user_image::kDefaultImageResourceIDs
        [user->image_index()];
    load_image_from_resource(resource_id);
  } else if (user->image_is_stub()) {
    load_image_from_resource(IDR_LOGIN_DEFAULT_USER);
  }

   return avatar;
 }
