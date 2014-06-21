#ifndef SERVERBASE_H
#define SERVERBASE_H
#include "NetworkService.h"
#include "CoreIOService.h"
class ServerBase
{
	typedef boost::asio::io_service io_service;
public:
	ServerBase()
	try:	_coreio_service(new CoreIO_Service),
			_network_service(new TCPNetworkService(_coreio_service->GetIOService()))
	{
		//constructor
	}
    catch(std::bad_alloc &e)
	{

	}
	virtual void Start() = 0;
	virtual void Shutdown() = 0;

protected:
	ServerBase(const ServerBase&){}
	boost::shared_ptr<CoreIO_Service> _coreio_service;
	boost::shared_ptr<NetworkService> _network_service;
};
#endif