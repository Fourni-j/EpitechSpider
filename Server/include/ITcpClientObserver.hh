
#ifndef _TCP_CLIENT_OBSERVER_HH_
# define _TCP_CLIENT_OBSERVER_HH_

namespace	Spider
{
  class	ITcpClientObserver
  {
  public:
    virtual	~ITcpClientObserver() {};

    virtual void	onClientStart() = 0;
    virtual void	onMessageRead(char*, size_t) = 0;
    virtual void	onTimeoutExpired() = 0;
  };
}

#endif
