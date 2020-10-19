#ifndef PASTEBIN_HPP_
#define PASTEBIN_HPP_

#include "hort/interface.hpp"

/// Forward Matches:
///  https://pastebin.com/u/user   download_user
///  https://pastebin.com/2uDians0 download_paste
HORT_INTERFACE(Pastebin) {
public:
  Pastebin();

  /// \brief Download all authors pastes.
  /// \return True if paste is already downloaded.
  int download_paste(const std::string& id);

  /// \brief Download paste given it's id, title and authors username.
  /// \return True if paste is already downloaded.
  int download_paste(const std::string& id,
                     std::string title,
                     const std::string& username);

  /// \brief Download paste given it's id.
  void download_user(const std::string& username);

private:
  /// \parameters paste
  const std::string url_rawpaste = "https://pastebin.com/raw/{}";

  /// \parameters user page_number
  const std::string url_user = "https://pastebin.com/u/{}/{}";

  const hort::re::regex re_pastes{R"regex(\ <a href="/(\w*?)">(.*?)</a>)regex"};
};

#endif /* PASTEBIN_HPP_ */
