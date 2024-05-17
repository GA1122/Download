PartialMagnificationController::~PartialMagnificationController() {
  CloseMagnifierWindow();

   Shell::GetInstance()->RemovePreTargetHandler(this);
 }
