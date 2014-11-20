
#ifndef _HEADER_HH_
# define _HEADER_HH_

# include <cstdint>
# include "spider.hh"

namespace	Spider
{
  class	Header
  {
  public:
    Header();
    Header(struct s_header);

    std::size_t	getLength();

    /* setter */
    void	setType(std::uint32_t);
    void	setIdClient(std::int32_t);
    void	setTokenId(std::uint32_t);
    void	setMsgId(std::uint32_t);
    void	setSize(std::uint32_t);
    void	setStruct(struct s_header);
    void	setData(char*);

    /* getter */
    std::uint32_t	getType() const;
    std::int32_t	getIdClient() const;
    std::uint32_t	getTokenId() const;
    std::uint32_t	getMsgId() const;
    std::uint32_t	getSize() const;
    struct s_header	getStruct() const;
    char*		getData();

  private:
    std::uint32_t	_type;
    std::int32_t	_idClient;
    std::uint32_t	_tokenId;
    std::uint32_t	_msgId;
    std::uint32_t	_size;
  };
}

#endif
