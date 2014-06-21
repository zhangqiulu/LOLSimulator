#ifndef COREIOSERVICE_H
#define COREIOSERVICE_H
#include "Require.h"
class CoreIO_Service
{
	typedef boost::asio::io_service io_service;
public:
	CoreIO_Service()
	try	:	_io_service(new io_service)
		,	_worker_threads(new boost::thread_group)
	{

	}
	catch(std::exception &e)
	{

	}
	boost::shared_ptr<io_service> GetIOService()
	{
		return _io_service;
	}
	virtual void ServiceRun(int threadNum = 0,boost::function<void()> f = NULL)
	{
		if(threadNum == 0 )
			_io_service->run();
		else
		{
			for (int i = 0;i != threadNum; i++)
			{
				if(f ==NULL)			
					f = boost::bind(&CoreIO_Service::workerthread,this,_io_service);
				_worker_threads->create_thread(f);
			}
		}
		//_worker_threads->join_all();
	}
	virtual void Servicestop()
	{
		_io_service->stop();
	}
	virtual void PostMission(boost::function<void()> f = NULL)
	{
		if(f != NULL)
		{
			_io_service->post(f);
		}
	}
protected:
	void workerthread(boost::shared_ptr<io_service> &io_service)
	{
		try
		{
			boost::system::error_code ec;
			io_service->run(ec);
		}
		catch(std::exception &e)
		{

		}
	}
private:
	CoreIO_Service(const CoreIO_Service&){}
	CoreIO_Service& operator=(const CoreIO_Service&){}
	boost::shared_ptr<io_service> _io_service;
	boost::shared_ptr<boost::thread_group> _worker_threads;
};
#endif