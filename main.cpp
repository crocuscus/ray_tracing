#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include <omp.h>

#include <QImage>

#include "scene.h"
#include "plane.h"
#include "base_geom.h"

using namespace std;

int main (int argc, const char** argv) {
  std::unordered_map<std::string, std::string> cmdLineParams;

  for (int i = 0; i < argc; i++) {
    std::string key(argv[i]);

    if(key.size() > 0 && key[0] == '-')
    {
      if(i != argc - 1) // not last argument
      {
        cmdLineParams[key] = argv[i + 1];
        i++;
      }
      else
        cmdLineParams[key] = "";
    }
  }
  std::string outFilePath = "zout.bmp";
  if (cmdLineParams.find("-out") != cmdLineParams.end())
    outFilePath = cmdLineParams["-out"];

  int threads = 6;
  if (cmdLineParams.find("-threads") != cmdLineParams.end())
      threads = atoi(cmdLineParams["-threads"].c_str());


  int sceneId = 0;
  int flag = 1;
  if (cmdLineParams.find("-threads") != cmdLineParams.end())
      flag = atoi(cmdLineParams["-scene"].c_str());
  if (flag > 1)
      return 0;

  double ratio = Scene::init_scenes();

  cout << "get ratio : " << ratio << endl;

  #ifdef _OPENMP
      printf("OpenMP is supported! %d \n\n", _OPENMP);
      omp_set_num_threads(threads);
  #endif

  int height = 500;
  if (cmdLineParams.find("-h") != cmdLineParams.end())
    height = atoi(cmdLineParams["-h"].c_str());
  int width = static_cast<int>(ratio * height);

  #ifdef _OPENMP
  long double time_start = static_cast<long double>(omp_get_wtime());
  #endif
  int anti_aliasing = 1;
  if (cmdLineParams.find("-a") != cmdLineParams.end())
    anti_aliasing = atoi(cmdLineParams["-a"].c_str());
  QImage img = Scene::draw_scene(sceneId, width, height, anti_aliasing);
  #ifdef _OPENMP
  long double time_end = static_cast<long double>(omp_get_wtime());
  printf(" %.2Lf\n", (time_end - time_start));
  #endif
  img.save(QString(outFilePath.c_str()), "BMP", 1);

  std::cout << "end." << std::endl;
  return 0;
}
