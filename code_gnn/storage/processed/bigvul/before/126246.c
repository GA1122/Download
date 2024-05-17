void Browser::TabStripEmpty() {
  MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&Browser::CloseFrame, weak_factory_.GetWeakPtr()));
}
