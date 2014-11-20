
#include <arpa/inet.h>
#include <cstring>
#include "Header.hh"

#include <iostream>
namespace	Spider
{

  Header::Header()
  {
    _type = RESP_KO;
    _idClient = 0;
    _tokenId = 0;
    _msgId = 0;
    _size = 0;
  }

  Header::Header(struct s_header structure)
  {
    _type = ntohl(structure.type);
    _idClient = static_cast<int32_t>(ntohl(structure.id_client));
    _tokenId = ntohl(structure.token_id);
    _msgId = ntohl(structure.msg_id);
    _size = ntohl(structure.size);
  }

  std::size_t	Header::getLength()
  {
    return (sizeof(struct s_header));
  }

  /* setter */
  void	Header::setType(std::uint32_t type) { _type = type; }
  void	Header::setIdClient(std::int32_t idClient) { _idClient = idClient; }
  void	Header::setTokenId(std::uint32_t tokenId) { _tokenId = tokenId; }
  void	Header::setMsgId(std::uint32_t msgId) { _msgId = msgId; }
  void	Header::setSize(std::uint32_t size) { _size = size; }

  void	Header::setStruct(struct s_header structure)
  {
    _type = ntohl(structure.type);
    _idClient = static_cast<int32_t>(ntohl(structure.id_client));
    _tokenId = ntohl(structure.token_id);
    _msgId = ntohl(structure.msg_id);
    _size = ntohl(structure.size);
  }

  void	Header::setData(char* data)
  {
    struct s_header	header;

    memcpy(&header, data, this->getLength());
    this->setStruct(header);
  }

  /* getter */
  std::uint32_t	Header::getType() const { return (_type); }
  std::int32_t	Header::getIdClient() const { return (_idClient); }
  std::uint32_t	Header::getTokenId() const { return (_tokenId); }
  std::uint32_t	Header::getMsgId() const { return (_msgId); }
  std::uint32_t	Header::getSize() const { return (_size); }

  struct s_header	Header::getStruct() const
  {
    struct s_header	ret;

    ret.id_client = htonl(_idClient);
    ret.token_id = htonl(_tokenId);
    ret.type = htonl(_type);
    ret.msg_id = htonl(_msgId);
    ret.size = htonl(_size);
    return (ret);
  }

  char*	Header::getData()
  {
    char*		data = new char[this->getLength()];
    struct s_header	*tmp = new struct s_header;

    *tmp = this->getStruct();
    memcpy(data, tmp, this->getLength());
    delete (tmp);
    return (data);
  }
}
