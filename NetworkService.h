#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H
#include "Require.h"
	
class NetworkService
{
	typedef boost::asio::io_service io_service;
public:
	struct NetworkMessage
	{
		const unsigned MAXLENGTH = 1024;
		NetworkMessage()
		{
			memset(data,0,MAXLENGTH);
		}
		char data[MAXLENGTH];
	};
	NetworkService(boost::shared_ptr<io_service> io)
	try	:	_io_service(io)
	{

	}
	catch(std::exception &e)
	{
		throw std::exception("NetService error");
	}
	NetworkService(const NetworkService& ns)
		:	_io_service(ns._io_service)
	{

	}
	virtual ~NetworkService(){}
	virtual boost::shared_ptr<io_service> GetIOService()
	{
		return _io_service;
	}
protected:
	boost::shared_ptr<io_service> _io_service;
};
class TCPNetworkService	:	public NetworkService
{
	typedef boost::asio::ip::tcp::socket socket;
	typedef boost::asio::io_service io_service;
	typedef boost::asio::ip::tcp::resolver resolver;
public:
	TCPNetworkService(boost::shared_ptr<io_service> io)
		:	NetworkService(io),
			_resolver(*io),
			_sokect(*io)
	{

	}

	virtual ~TCPNetworkService()
	{

	}
// 	boost::shared_ptr<io_service> GetIOService()
// 	{
// 		return NetworkService::GetIOService();
// 	}
	socket& GetSocket()
	{
		return _sokect;
	}
	void OpenAndBind(resolver::endpoint_type& endpoint)
	{
		_sokect.open(boost::asio::ip::tcp::v4());
		_sokect.bind(endpoint);
	}
	boost::system::error_code Syn_Connect(const boost::asio::ip::tcp::endpoint& endpoint)
	{
		boost::system::error_code ec;
		_sokect.connect(endpoint,ec);
		return ec;
	}
	boost::system::error_code Syn_Connect(const std::string& host, unsigned short prot)
	{
		return Syn_Connect(_make_endpoint(host,prot));
	}
	template<class ConnectHandler>
	void Asyn_Connect(const boost::asio::ip::tcp::endpoint& endpoint,ConnectHandler handler)
	{
		_sokect.async_connect(endpoint,handler);
	}
	template<class ConnectHandler>
	void Asyn_Connect(const std::string& host, unsigned short prot,ConnectHandler handler)
	{
		_sokect.async_connect(_make_endpoint(host,prot),handler);
	}
	void Syn_Read(std::string &s)
	{
		char c[1024];
		_sokect.receive(boost::asio::buffer(c));
		s = c;
	}
	template<class data,class ConnectHandler>
	void Asyn_Read(data* data,ConnectHandler handler)
	{
		_sokect.async_receive(boost::asio::buffer(data->data),boost::bind(handler,data,boost::asio::placeholders::bytes_transferred));
	}
	void Syn_Write(std::string &s)
	{
		_sokect.send(boost::asio::buffer(s));
	}
	template<class ConnectHandler>
	void Asyn_Write(std::string &s,ConnectHandler handler)
	{
		_sokect.async_send(boost::asio::buffer(s),handler);
	}
	void SocketShutdown(boost::asio::ip::tcp::socket::shutdown_type what)
	{
		_sokect.shutdown(what);
	}
	void SocketClose()
	{
		_sokect.close();
	}
protected:
	resolver::endpoint_type _make_endpoint(const std::string& host, unsigned short prot)
	{
		boost::asio::ip::address add(boost::asio::ip::address::from_string(host));
		return boost::asio::ip::tcp::endpoint(add,prot);
	}
private:
	resolver _resolver;
	socket _sokect;
	TCPNetworkService(const TCPNetworkService& ns)
		:	NetworkService(ns._io_service),
			_resolver(*(ns._io_service)),
			_sokect(*(ns._io_service))
	{
	}
};

class TCPServerNetworkService	:	public  TCPNetworkService
{
	typedef boost::asio::ip::tcp::acceptor acceptor;
	typedef boost::asio::io_service io_service;
	typedef boost::asio::ip::tcp tcp;
public:
	TCPServerNetworkService(boost::shared_ptr<io_service> io)	
		:	TCPNetworkService(io),
			_acceptor(*io)
	{

	}
	TCPServerNetworkService(boost::shared_ptr<io_service> io,unsigned short port)	
		:	TCPNetworkService(io),
			_acceptor(*io,tcp::endpoint(tcp::v4(), port))
	{
		
	}
	TCPServerNetworkService(boost::shared_ptr<io_service> io,const resolver::endpoint_type& endpoint )
		:	TCPNetworkService(io),
			_acceptor(*io,endpoint)
	{

	}
	virtual ~TCPServerNetworkService()
	{

	}
	void OpenAndListen(const resolver::endpoint_type& endpoint,int backlog =0)
	{
		_acceptor.open(endpoint.protocol());
		_acceptor.bind(endpoint);
		_acceptor.listen(backlog);
	}
	acceptor& GetAcceptor()
	{
		return _acceptor;
	}
	boost::system::error_code Syn_Accept()
	{
		boost::system::error_code ec;
		_acceptor.accept(GetSocket(),ec);
		return ec;
	}
	template<class AcceptHandler>
	void Asyn_Accept(AcceptHandler handler)
	{
		_acceptor.async_accept(GetSocket(),handler);
	}
	void AcceptotClose()
	{
		_acceptor.close();
	}
private:
	TCPServerNetworkService(const TCPServerNetworkService& tsns)
		:	TCPNetworkService(tsns._io_service),
			_acceptor(*(tsns._io_service))
	{

	}
	acceptor _acceptor;
};
#endif