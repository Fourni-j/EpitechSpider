
#ifndef _MAGIC_HEADER_HH_
# define _MAGIC_HEADER_HH_

# include <string>
# include "spider.hh"

namespace	Spider
{
  class	MagicHeader
  {
  public:
    MagicHeader();
    MagicHeader(struct s_magic_header);
    ~MagicHeader();

    std::size_t	getLength();

    /* setter */
    void	setName(const std::string&);
    void	setMagicNb(std::uint32_t);
    void	setVersion(std::uint32_t);
    void	setSubversion(std::uint32_t);
    void	setStruct(struct s_magic_header);
    void	setData(char*);

    /* getter */
    const std::string&		getName() const;
    std::uint32_t		getMagicNb() const;
    std::uint32_t		getVersion() const;
    std::uint32_t		getSubversion() const;
    struct s_magic_header	getStruct() const;
    char*			getData();

  private:
    std::string		_name;
    std::uint32_t	_magicNb;
    std::uint32_t	_version;
    std::uint32_t	_subversion;
  };
}

#endif
