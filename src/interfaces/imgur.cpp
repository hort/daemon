#include "interfaces/imgur.hpp"

Imgur::Imgur()
  : hort::Interface{"imgur",
    { { R"regex(^(?:https?://)?imgur.com/(\w+)/?$)regex",
        [this] (auto &v) {
          download_gallery(v[0]);
        } }
  } } {}

int Imgur::download_gallery(const std::string &id) {
  auto response = session.get(url_imgs, id);
  auto images = json::parse(re_hashes.findall(response.body).at(-1));

  int index = 1;
  hort::archive archive(hortpath + id + ".zip");
  for (const auto& image : images) {
    auto hash = image["hash"].get<std::string>();
    auto ext = image["ext"].get<std::string>();
    auto filepath = std::to_string(index) + ext;
    archive.add(filepath, session.get(url_img, hash, ext).body);
    index += 1;
  }

  return 0;
}
