void AXNodeObject::alterSliderValue(bool increase) {
  if (roleValue() != SliderRole)
    return;

  float value = valueForRange();
  float step = stepValueForRange();

  value += increase ? step : -step;

  setValue(String::number(value));
  axObjectCache().postNotification(getNode(),
                                   AXObjectCacheImpl::AXValueChanged);
}
