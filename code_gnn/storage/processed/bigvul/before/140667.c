void Transform(PP_Instance instance, PP_PrivatePageTransformType type) {
  void* object =
      pp::Instance::GetPerInstanceObject(instance, kPPPPdfInterface);
  if (object) {
    OutOfProcessInstance* obj_instance =
        static_cast<OutOfProcessInstance*>(object);
    switch (type) {
      case PP_PRIVATEPAGETRANSFORMTYPE_ROTATE_90_CW:
        obj_instance->RotateClockwise();
        break;
      case PP_PRIVATEPAGETRANSFORMTYPE_ROTATE_90_CCW:
        obj_instance->RotateCounterclockwise();
        break;
    }
  }
}
