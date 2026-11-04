#include "callbacks.hpp"
#include <chrono>

double dt1, dt2, dt3, dt4, dt5, frames = 0;

static std::array<SDL_Vertex, vertlen> init_verts()
{
  std::array<SDL_Vertex, vertlen> vert;
  static constexpr SDL_FColor kcolors[2] = {
    {1.f, 1.f, 1.f, 1.f},
    {0.f, 0.f, 0.f, 0.f},
  };
  int i = 0;
  for (SDL_Vertex &v : vert)
  {
    v.color = kcolors[(i++ % 3 == 0) ? 1 : 0];
    v.tex_coord = {0, 0};
  }
  return vert;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  mystuff *stuff = static_cast<mystuff *>(appstate);
  Cluster &cluster = *(stuff->cluster);

  auto t0 = std::chrono::steady_clock::now();
  auto t1 = std::chrono::steady_clock::now();

  static std::array<SDL_Vertex, vertlen> verts = init_verts();

  const float halfW = static_cast<float>(stuff->w) * 0.5f;
  const float halfH = static_cast<float>(stuff->h) * 0.5f;

  for (size_t i = 0; i < n; i++)
  {
    float &star_z = cluster.z[i];
    star_z = (star_z > Z_WRAP) ? star_z - Z_WRAP : maxz;
    const float inv_z = M / star_z;
    const float tx = (cluster.x[i] * inv_z + 1.f) * halfW;
    const float ty = (cluster.y[i] * inv_z + 1.f) * halfH;
    const float tr = R * star_z + T;
    const float tr2 = tr * inv15;
    SDL_Vertex *v = &verts[i * 12];
    v[0].position = {tx - tr, ty};
    v[1].position = {tx + tr2, ty + tr2};
    v[2].position = {tx + tr2, ty - tr2};
    v[3].position = {tx, ty + tr};
    v[4].position = {tx + tr2, ty - tr2};
    v[5].position = {tx - tr2, ty - tr2};
    v[6].position = {tx + tr, ty};
    v[7].position = {tx - tr2, ty - tr2};
    v[8].position = {tx - tr2, ty + tr2};
    v[9].position = {tx, ty - tr};
    v[10].position = {tx - tr2, ty + tr2};
    v[11].position = {tx + tr2, ty + tr2};
  }
  auto t2 = std::chrono::steady_clock::now();

  SDL_SetRenderDrawColor(stuff->Renderer, 0, 0, 0, 255);
  SDL_RenderClear(stuff->Renderer);
  auto t3 = std::chrono::steady_clock::now();

  SDL_SetRenderDrawBlendMode(stuff->Renderer,SDL_BLENDMODE_ADD);
  if (!SDL_RenderGeometry(stuff->Renderer, NULL, verts.data(), verts.size(), NULL, 0))
  {
    SDL_Log("RenderGeometry: %s", SDL_GetError());
  }

  auto t4 = std::chrono::steady_clock::now();

  if (!SDL_RenderPresent(stuff->Renderer))
  {
    SDL_Log("Present: %s", SDL_GetError());
  }

  auto t5 = std::chrono::steady_clock::now();
  dt1 += double(std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count());
  dt2 += double(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
  dt3 += double(std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count());
  dt4 += double(std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count());
  dt5 += double(std::chrono::duration_cast<std::chrono::microseconds>(t5 - t4).count());
  frames++;

  return SDL_APP_CONTINUE;
}
