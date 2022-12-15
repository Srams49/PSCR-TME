#pragma once
#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include "Barrier.h"
#include "Pool.h"
#include "Queue.h"
#include <random>
#include "Job.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>

namespace pr {

	using namespace std;

	//add in case of using one or more of those function

	void fillScene(Scene & scene, default_random_engine & re) {
		// Nombre de spheres (rend le probleme plus dur)
		const int NBSPHERES = 250;

		// on remplit la scene de spheres colorees de taille position et couleur aleatoire
		uniform_int_distribution<int> distrib(0, 200);
		uniform_real_distribution<double> distribd(-200, 200);
		for (int i = 0; i < NBSPHERES; i++) {
			// position autour de l'angle de la camera
			// rayon entre 3 et 33, couleur aleatoire
			// distrib(re) rend un entier aleatoire extrait de re
			scene.add(Sphere({50+distribd(re),50 + distribd(re),120 + distribd(re) }, double(distrib(re)%30) + 3.0, Color::random()));
		}
		// quelques spheres de plus pour ajouter du gout a la scene
		scene.add(Sphere({50,50,40},15.0,Color::red));
		scene.add(Sphere({100,20,50},55.0,Color::blue));

	}

	// return the index of the closest object in the scene that intersects "ray"
	// or -1 if the ray does not intersect any object.
	int findClosestInter(const Scene & scene, const Rayon & ray) {
		auto minz = std::numeric_limits<float>::max();
		int targetSphere = -1;
		int index = 0;
		for (const auto & obj : scene) {
			// rend la distance de l'objet a la camera
			auto zinter = obj.intersects(ray);
			// si intersection plus proche  ?
			if (zinter < minz) {
				minz = zinter;
				targetSphere = index;
			}
			index++;
		}
		return targetSphere;
	}

	// Calcule l'angle d'incidence du rayon à la sphere, cumule l'éclairage des lumières
	// En déduit la couleur d'un pixel de l'écran.
	Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights) {
		Color finalcolor = obj.getColor();

		// calcul du rayon et de sa normale a la sphere
		// on prend le vecteur de la camera vers le point de l'ecran (dest - origine)
		// on le normalise a la longueur 1, on multiplie par la distance à l'intersection
		Vec3D rayInter = (ray.dest - ray.ori).normalize() * obj.intersects(ray);
		// le point d'intersection
		Vec3D intersection = rayInter + camera;
		// la normale a la sphere au point d'intersection donne l'angle pour la lumiere
		Vec3D normal = obj.getNormale(intersection);
		// le niveau d'eclairage total contribue par les lumieres 0 sombre 1 total lumiere
		double dt = 0;
		// modifier par l'eclairage la couleur
		for (const auto & light : lights) {
			// le vecteur de la lumiere au point d'intersection
			Vec3D tolight = (light - intersection);
			// si on est du bon cote de la sphere, i.e. le rayon n'intersecte pas avant de l'autre cote
			if (obj.intersects(Rayon(light,intersection)) >= tolight.length() - 0.05 ) {   //  epsilon 0.05 for double issues
				dt += tolight.normalize() & normal ; // l'angle (scalaire) donne la puissance de la lumiere reflechie
			}
		}
		// eclairage total
		finalcolor = finalcolor * dt + finalcolor * 0.2; // *0.2 = lumiere speculaire ambiante

		return finalcolor;
	}

	// produit une image dans path, représentant les pixels.
	void exportImage(const char * path, size_t width, size_t height, Color * pixels) {
		// ppm est un format ultra basique
		ofstream img (path);
		// P3 signifie : les pixels un par un en ascii
		img << "P3" << endl; // ascii format, colors
		// largeur hauteur valeur max d'une couleur (=255 un char)
		img << width  << "\n"<< height << "\n" << "255" << endl;
		// tous les pixels au format RGB
		for (size_t  y = 0 ; y < height ; y++) {
			for (size_t x =0 ; x < width ; x++) {
				Color & pixel = pixels[x*height + y];
				img << pixel << '\n';
			}
		}
		// oui ca fait un gros fichier :D
		img.close();
	}

	// NB : en francais pour le cours, preferez coder en english toujours.
	// pas d'accents pour eviter les soucis d'encodage



class Job {
public:
	virtual void run () = 0;
	virtual ~Job() {};
};

class JobPixel : public Job{
	private:
		
		int x;
		int y;
		const Scene::screen_t * screen;
		Scene *scene;
		Color * pixels;
		std::vector<Vec3D> lights;
		Barrier*  Blockeur;

	public:

		JobPixel( int i_x, int i_y,	const Scene::screen_t * i_screen,	Scene * i_scene, Color * i_pixels, std::vector<Vec3D> i_lights, Barrier * i_Blockeur):
			x(i_x),
			y(i_y),
			screen(i_screen),
			scene(i_scene),
			pixels(i_pixels),
			lights(i_lights),
			Blockeur(i_Blockeur)
		{}


		void run(){// on le fait par ligne de pixel (en suprimant le for => pixel par pixel)
			for (int  y = 0 ; y < scene->getHeight() ; y++) {
				// le point de l'ecran par lequel passe ce rayon
				auto & screenPoint = (*screen)[y][x];
				// le rayon a inspecter
				Rayon  ray(scene->getCameraPos(), screenPoint);

				int targetSphere = findClosestInter(*scene, ray);

				if (targetSphere == -1) {
					// keep background color
					continue ;
				} else {
					const Sphere & obj = *(scene->begin() + targetSphere);
					// pixel prend la couleur de l'objet
					Color finalcolor = computeColor(obj, ray, scene->getCameraPos(), lights);
					// le point de l'image (pixel) dont on vient de calculer la couleur
					Color & pixel = pixels[y*scene->getHeight() + x];
					// mettre a jour la couleur du pixel dans l'image finale.
					pixel = finalcolor;
				}

			}
			Blockeur -> done();
		}
		

};







// Job concret : exemple

/**
class SleepJob : public Job {
	int calcul (int v) {
		std::cout << "Computing for arg =" << v << std::endl;
		// traiter un gros calcul
		this_thread::sleep_for(1s);
		int ret = v % 255;
		std::cout << "Obtained for arg =" << arg <<  " result " << ret << std::endl;
		return ret;
	}
	int arg;
	int * ret;
public :
	SleepJob(int arg, int * ret) : arg(arg), ret(ret) {}
	void run () {
		* ret = calcul(arg);
	}
	~SleepJob(){}
};
**/

}