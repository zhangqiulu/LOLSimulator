#include <memory>
#include <iostream>
#include <string>
#include "ServerBase.h"
#include "CoreIOService.h"
#include "NetworkService.h"
using namespace  std;

struct msg
{
	char data[1024];
};
void WorkThread(boost::shared_ptr<boost::asio::io_service> coreio)
{
	std::cout<<"["<<boost::this_thread::get_id()<<":"<<"]"<<endl;
	coreio->run();
}
void MissionOne()
{
	while(1)
	{
		cout<<"Mission success"<<endl;
		//boost::this_thread::sleep(1000);
	}
}
void serverread_handler(const boost::system::error_code& error,msg* msg,size_t );
void accept_handler(const boost::system::error_code& error,boost::shared_ptr<NetworkService> server)
{
	if (!error)
	{
		// Accept succeeded.
		cout<<"Yes"<<endl;
		cout<<boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->GetSocket().remote_endpoint()<<endl;
		msg* buffer = new msg();
		memset(buffer->data,0,1024);
		boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->Asyn_Read(buffer,(boost::bind(&serverread_handler,error,_1,_2)));
	}
	else
	{
		cout<<error.value()<<endl;
	}
	//boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->Asyn_Accept(boost::bind(&accept_handler,boost::asio::placeholders::error,server));
}
void connect_handler(const boost::system::error_code& error,boost::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
	if(!error)
	{
		cout<<"connected to server"<<endl;
		//cout<<socket.remote_endpoint()<<endl;
	}
	else
	{
		cout<<error.value()<<endl;
	}
}
void serverread_handler(const boost::system::error_code& error,msg* msg,size_t st)
{
	if(!error)
	{		
		string s = msg->data;
		cout<<s<<endl;
	}
	else
	{
		cout<<error.value()<<endl;
	}
}
void main()
{
	boost::asio::io_service io;
	//boost::asio::io_service::work work(io);

	//io.run();

 	boost::shared_ptr<CoreIO_Service> core_io(new CoreIO_Service);
 	boost::asio::io_service::work work(*(core_io->GetIOService()));
	
// 	core_io->ServiceRun(5,boost::bind(&WorkThread,core_io->GetIOService()));
// 	int a = 0;
// 	while (cin>>a)
// 	{
// 		if(a == 1)
// 			core_io->PostMission(&MissionOne);
// 		if(a == 404)
// 		{
// 			core_io->Servicestop();
// 			break;
// 		}
//	}
	//server
 	boost::asio::ip::tcp::endpoint serverendpoint(boost::asio::ip::address::from_string("10.141.249.241"), 2221);
  	boost::shared_ptr<NetworkService> server(new TCPServerNetworkService(core_io->GetIOService(),serverendpoint));

// 
// 	//boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->OpenAndListen(serverendpoint);
 	boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->Asyn_Accept(boost::bind(&accept_handler,boost::asio::placeholders::error,server));


	//boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->Asyn_Read(buffer,boost::bind(&serverread_handler,boost::asio::placeholders::error,buffer));
 	boost::this_thread::sleep(boost::posix_time::seconds(1));
  	//core_io->ServiceRun();
	//client
	boost::shared_ptr<NetworkService> client(new TCPNetworkService(core_io->GetIOService()));
	boost::asio::ip::tcp::endpoint clientendpoint(boost::asio::ip::tcp::v4(), 1239);
	boost::dynamic_pointer_cast<TCPNetworkService>(client)->OpenAndBind(clientendpoint);
	boost::shared_ptr<boost::asio::ip::tcp::socket> asocket(new boost::asio::ip::tcp::socket(io));
	boost::dynamic_pointer_cast<TCPNetworkService>(client)->Syn_Connect(serverendpoint/*,boost::bind(&connect_handler,boost::asio::placeholders::error,asocket)*/);
	std::string s = "hello Phx";
	boost::dynamic_pointer_cast<TCPNetworkService>(client)->Syn_Write(s);
// 	catch(std::exception &e)
// 	{
// 		cout<<e.what()<<endl;
// 	}
	core_io->ServiceRun();
	try
	{
		boost::dynamic_pointer_cast<TCPNetworkService>(client)->SocketShutdown(boost::asio::ip::tcp::acceptor::shutdown_type::shutdown_both);
		boost::dynamic_pointer_cast<TCPNetworkService>(client)->SocketClose();
		boost::dynamic_pointer_cast<TCPNetworkService>(server)->SocketShutdown(boost::asio::ip::tcp::acceptor::shutdown_type::shutdown_both);
		boost::dynamic_pointer_cast<TCPNetworkService>(server)->SocketClose();
		boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->AcceptotClose();
		
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
	}
// 	boost::dynamic_pointer_cast<TCPNetworkService>(server)->SocketShutdown(boost::asio::ip::tcp::acceptor::shutdown_type::shutdown_both);
// 	boost::dynamic_pointer_cast<TCPNetworkService>(server)->SocketClose();
// 	boost::dynamic_pointer_cast<TCPServerNetworkService>(server)->AcceptotClose();
	system("pause");
}