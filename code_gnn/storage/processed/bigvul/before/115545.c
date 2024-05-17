TestController& TestController::shared()
{
    ASSERT(controller);
    return *controller;
}
