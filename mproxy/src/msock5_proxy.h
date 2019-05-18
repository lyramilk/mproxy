#ifndef _lyramilk_msock5_proxy_h_
#define _lyramilk_msock5_proxy_h_

#include <libmilk/netaio.h>
#include <libmilk/netproxy.h>
#include <stdlib.h>


namespace lyramilk{ namespace mproxy
{

	class msock5_proxy:public lyramilk::netio::aioproxysession
	{
		enum {
			s0,
			s1,
		} status;


		const static unsigned char CMD_CONNECT = 1;
		const static unsigned char CMD_BIND = 1;


		const static unsigned char ATYPE_IPV4 = 1;
		const static unsigned char ATYPE_DOMAINNAME = 3;
		const static unsigned char ATYPE_IPV6 = 4;
	  public:
		msock5_proxy();
	  	virtual ~msock5_proxy();
		virtual bool onrequest(const char* cache, int size,int* bytesused, lyramilk::data::ostream& os);
	};

}}
#endif
