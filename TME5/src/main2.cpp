#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include "Barrier.h"
#include "Pool.h"
#include "Queue.h"
#include <random>
#include "Job.h"
#define  NB_THREADS  16 //nb coeur à la maison (reduire a la fac) 

using namespace std;
using namespace pr;

int main () {


    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	// on pose une graine basee sur la date
	default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	// definir la Scene : resolution de l'image
	Scene scene (1000,1000);
	// remplir avec un peu d'aléatoire
	fillScene(scene, re);
	
	// lumieres 
	vector<Vec3D> lights;
	lights.reserve(3);
	lights.emplace_back(Vec3D(50, 50, -50));
	lights.emplace_back(Vec3D(50, 50, 120));
	lights.emplace_back(Vec3D(200, 0, 120));

	// les points de l'ecran, en coordonnées 3D, au sein de la Scene.
	// on tire un rayon de l'observateur vers chacun de ces points
	const Scene::screen_t & screen = scene.getScreenPoints();

	// Les couleurs des pixels dans l'image finale
	Color * pixels = new Color[scene.getWidth() * scene.getHeight()];

    Pool piscine = Pool(15);
    piscine.start(NB_THREADS);
    Barrier b = Barrier(scene.getWidth());

	// pour chaque pixel, calculer sa couleur
	for (int x =0 ; x < scene.getWidth() ; x++) {
		// parallésisation
        piscine.submit(new JobPixel(x,0, &screen, &scene, pixels, lights, &b));

	}
    
    b.waitFor();
    piscine.stop();
	
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    std::cout << "Total time "
	              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
	              << "ms.\n";

	exportImage("toto.ppm",scene.getWidth(), scene.getHeight() , pixels);

	return 0;
}