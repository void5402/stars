#include "callbacks.hpp"
#include <chrono>

double dt1, dt2, dt3, dt4, dt5, frames = 0;

static std::array<SDL_Vertex, vertlen> init_verts() {
  std::array<SDL_Vertex, vertlen> vert;
  for (size_t i = 0; i < vertlen; i += 4) {
    vert[i + 0] = {{0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 0.f}};
    vert[i + 1] = {{0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 0.f}};
    vert[i + 2] = {{0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f}};
    vert[i + 3] = {{0.f, 0.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 1.f}};
  }
  return vert;
}
static std::array<int, indilen> init_indi() {
  std::array<int, indilen> indicies;
  for (int l = 0; l < n; l++) {
    const int m = l * 4;
    const size_t i = static_cast<unsigned int>(l * 6);
    indicies[i + 0] = m + 0;
    indicies[i + 1] = m + 1;
    indicies[i + 2] = m + 2;
    indicies[i + 3] = m + 0;
    indicies[i + 4] = m + 2;
    indicies[i + 5] = m + 3;
  }
  return indicies;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  mystuff *stuff = static_cast<mystuff *>(appstate);
  Cluster &cluster = *(stuff->cluster);

  // control
  auto t0 = std::chrono::steady_clock::now();
  auto t1 = std::chrono::steady_clock::now();

  static std::array<SDL_Vertex, vertlen> verts = init_verts();
  static const std::array<int, indilen> indicies = init_indi();

  const float halfW = static_cast<float>(stuff->w) * 0.5f;
  const float halfH = static_cast<float>(stuff->h) * 0.5f;

  for (size_t i = 0; i < n; i++) {
    float &star_z = cluster.z[i];
    star_z = (star_z > Z_WRAP) ? star_z - Z_WRAP : maxz;
    const float inv_z = M / star_z;
    const float tx = (cluster.x[i] * inv_z + 1.f) * halfW;
    const float ty = (cluster.y[i] * inv_z + 1.f) * halfH;
    const float tr = R * star_z + T;
    SDL_Vertex *v = &verts[i * 4];
    v[0].position = {tx - tr, ty - tr};
    v[1].position = {tx + tr, ty - tr};
    v[2].position = {tx + tr, ty + tr};
    v[3].position = {tx - tr, ty + tr};
  }
  auto t2 = std::chrono::steady_clock::now();

  SDL_SetRenderDrawColor(stuff->Renderer, 0, 0, 0, 255);
  SDL_RenderClear(stuff->Renderer);
  auto t3 = std::chrono::steady_clock::now();

  if (!SDL_RenderGeometry(stuff->Renderer, stuff->texture, verts.data(),
                          verts.size(), indicies.data(), indicies.size())) {
    SDL_Log("RenderGeometry: %s", SDL_GetError());
  }

  auto t4 = std::chrono::steady_clock::now();

  if (!SDL_RenderPresent(stuff->Renderer)) {
    SDL_Log("Present: %s", SDL_GetError());
  }

  auto t5 = std::chrono::steady_clock::now();
  dt1 += double(
      std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count());
  dt2 += double(
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
  dt3 += double(
      std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count());
  dt4 += double(
      std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count());
  dt5 += double(
      std::chrono::duration_cast<std::chrono::microseconds>(t5 - t4).count());
  frames++;

  return SDL_APP_CONTINUE;
}
