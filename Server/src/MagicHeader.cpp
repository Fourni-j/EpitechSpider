
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include "MagicHeader.hh"

namespace	Spider
{
  MagicHeader::MagicHeader()
  {
    _name = "";
    _magicNb = 0;
    _version = 0;
    _subversion = 0;
  }

  MagicHeader::MagicHeader(struct s_magic_header magicHeader)
  {
    _name = magicHeader.name;
    _magicNb = ntohl(magicHeader.magicNb);
    _version = ntohl(magicHeader.version);
    _subversion = ntohl(magicHeader.subversion);
  }

  MagicHeader::~MagicHeader()
  {

  }

  std::size_t	MagicHeader::getLength()
  {
    return (sizeof(struct s_magic_header));
  }

  /* setter */
  void	MagicHeader::setName(const std::string& name) { _name = name; }
  void	MagicHeader::setMagicNb(std::uint32_t magicNb) { _magicNb = magicNb; }
  void	MagicHeader::setVersion(std::uint32_t version) { _version = version; }
  void	MagicHeader::setSubversion(std::uint32_t subversion) { _subversion = subversion; }

  void	MagicHeader::setStruct(struct s_magic_header magicHeader)
  {
    _name = magicHeader.name;
    _magicNb = ntohl(magicHeader.magicNb);
    _version = ntohl(magicHeader.version);
    _subversion = ntohl(magicHeader.subversion);
  }

  void	MagicHeader::setData(char* data)
  {
    struct s_magic_header	magicHeader;

    memcpy(&magicHeader, data, this->getLength());
    this->setStruct(magicHeader);
  }

  /* getter */
  const std::string&	MagicHeader::getName() const { return (_name); }
  std::uint32_t		MagicHeader::getMagicNb() const { return (_magicNb); }
  std::uint32_t		MagicHeader::getVersion() const { return (_version); }
  std::uint32_t		MagicHeader::getSubversion() const { return (_subversion); }

  struct s_magic_header	MagicHeader::getStruct() const
  {
    struct s_magic_header	resp;

    memset(resp.name, '\0', sizeof(resp.name));
    for (size_t i = 0; i < _name.size(); i++)
      resp.name[i] = _name[i];
    resp.magicNb = htonl(_magicNb);
    resp.version = htonl(_version);
    resp.subversion = htonl(_subversion);
    return (resp);
  }

  char*	MagicHeader::getData()
  {
    char*			data = new char[this->getLength()];
    struct s_magic_header	*tmp = new struct s_magic_header;

    *tmp = this->getStruct();
    memcpy(data, tmp, this->getLength());
    delete (tmp);
    return (data);
  }
}
