#include "msock5_proxy.h"
#include "config.h"
#include <libmilk/stringutil.h>

namespace lyramilk{ namespace mproxy
{
	// msock5_proxy
	msock5_proxy::msock5_proxy()
	{
		status = s0;
	}

	msock5_proxy::~msock5_proxy()
	{
	}

	bool msock5_proxy::onrequest(const char* cache, int size,int* bytesused, lyramilk::data::ostream& os)
	{
		if(status == s0){
			unsigned char ver = (unsigned char)cache[0];
			if(ver == 5 && size > 2){
				unsigned char nmethod = (unsigned char)cache[1];
				if(size >= 2 + nmethod){
					for(int i=0;i<nmethod;++i){
						unsigned char xmethod = cache[1+i];
						if(xmethod == 0){
							//仅支持无密码
							os.write("\x05\x00",2);
							status = s1;
							*bytesused = 2 + nmethod;
							return true;
						}
					}
				}

			}
			os.write("\x05\xff",2);
			return false;
		}else if(status == s1){
			unsigned char cmd = (unsigned char)cache[1];

			if(cmd == CMD_CONNECT && size > 4){
				unsigned char address_type = (unsigned char)cache[3];
				if(address_type == ATYPE_IPV4 && size >= 10){
					sockaddr_in addr = {0};
					addr.sin_addr.s_addr = *(unsigned int*)(cache + 4);
					addr.sin_family = AF_INET;
					addr.sin_port = *(unsigned short*)(cache + 8);

					lyramilk::netio::aioproxysession_speedy* endpoint = lyramilk::netio::aiosession::__tbuilder<lyramilk::netio::aioproxysession_speedy>();
					if(endpoint->open(addr,200)){
						combine(endpoint);
						os.write("\x05\x00\x00\x01\x00\x00\x00\x00\x00\x00",10);
						//os.flush();
						*bytesused = 10;
						return true;
					}else{
						os.write("\x05\x03\x00\x01\x00\x00\x00\x00\x00\x00",10);
						*bytesused = 10;
						return true;
					}
				}
			}
		}else{
		}
		return false;
	}
}}
