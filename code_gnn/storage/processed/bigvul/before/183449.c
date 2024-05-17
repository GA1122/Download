  ResizeGripper::ResizeGripper(ResizeGripperDelegate* delegate)
    : delegate_(delegate), initial_position_(0) {
//     : delegate_(delegate),
//       initial_position_(0),
//       gripper_visible_(false) {
    ResourceBundle &rb = ResourceBundle::GetSharedInstance();
    SkBitmap* gripper_image = rb.GetBitmapNamed(IDR_RESIZE_GRIPPER);
  SetImage(gripper_image);
//    
//    
//    
//   SetImageSize(gfx::Size(gripper_image->width(), gripper_image->height()));
  }