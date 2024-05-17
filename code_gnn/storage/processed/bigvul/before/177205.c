 virtual void binderDied(const wp<IBinder> &) {
        mNotify->post();
 }
