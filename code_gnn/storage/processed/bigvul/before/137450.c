void MessageLoop::Quit() {
  DCHECK_EQ(this, current());
   pump_->Quit();
 }
