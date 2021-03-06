#pragma once

#include <string>
#include <vector>
#include <dirent.h>

struct direntry {
  std::string name;
  std::string type;
  std::string path;
};

/*******************************************************************************

Stores and operates on data related to directory listings

*******************************************************************************/
class DirListing {

  private:

    std::vector<direntry> _entries;
    std::string _path;

    /* Conversions */
   
    std::string nameFromDirent(dirent *pent) const;
    std::string typeFromDirent(dirent *pent) const;
    std::string typeFromStat(const std::string & path) const;

  public:

    static std::string separator;

    /* Constructors */

    DirListing();
    DirListing(bool sort_listing);
    DirListing(bool sort_listing, bool show_hidden);
    DirListing(const std::string & path);
    DirListing(const std::string & path, bool sort_listing);
    DirListing(const std::string & path, bool sort_listing, bool show_hidden);

    /* Create listing */

    int setFromPath(const std::string & path, bool sort_listing=true,
                    bool show_hidden=false);
    int setFromCwd(bool sort_listing=true, bool show_hidden=false);
    int navigateUp(bool sort_listing=true, bool show_hidden=false);

    /* Same as setFromPath, but also creates directory if it doesn't exist */

    int createFromPath(const std::string & path, bool sort_listing=true,
                       bool show_hidden=false);

    /* Sorts entries */
   
    void sort();

    /* Returns direntry by index or name */

    const direntry operator () (unsigned int idx) const;
    const direntry operator () (const std::string & name) const;

    /* Returns path */

    const std::string & path() const;

    /* Returns number of entries */

    unsigned int size() const;
};
