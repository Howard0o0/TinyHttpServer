#include <tcpconnection.h>
#include <unistd.h>

void TcpConnection::Disconnect() {
	close(this->io_watcher_->fd);
	this->io_watcher_->stop();
}
int TcpConnection::GetConnectionFd() const {
	return this->io_watcher_->fd;
}