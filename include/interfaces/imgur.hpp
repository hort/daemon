#ifndef IMGUR_HPP_
#define IMGUR_HPP_

#include "hort/interface.hpp"

HORT_INTERFACE(Imgur) {
public:
  Imgur();

  /// \brief Download entire gallery.
  int download_gallery(const std::string &id);

private:
  /// \parameters filename ext
  const std::string url_img  = "https://imgur.com/{}{}";

  /// \parameters id
  const std::string url_imgs = "https://imgur.com/a/{}";

  const hort::re::regex re_hashes{R"regex("images":(\[.*?]))regex"};
};

#endif // IMGUR_HPP_
