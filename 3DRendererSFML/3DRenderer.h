#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <cmath>

/*
	x : L'axe horizontal (largeur de l'écran)
	y : L'axe vertical   (hauteur de l'écran)
	z : La "profondeur"  (éloignement visuel)
*/

struct Point3D {
	float x, y, z;
};

struct Point2D {
	float x, y;
};

struct Vertex {
	int start, end;
};

class Renderer {
	// Private variables
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock Clock;
	float rotation;
	float deltaTime;
	float colorIndex;
	float FOV;
	int SCALE;
	std::vector<sf::Color> rainbowColors;
	std::vector<Point3D> cubePoints3D;
	std::vector<Point3D> pyramidPoints3D;
	std::vector<Point3D> diamondPyramidPoints3D;
	std::vector<Point3D> diamondPoints3D;
	std::vector<Point3D> squaredDonutPoints3D;
	std::vector<Vertex> cubeVertices;
	std::vector<Vertex> pyramidVertices;
	std::vector<Vertex> diamondPyramidVertices;
	std::vector<Vertex> diamondVertices;
	std::vector<Vertex> squaredDonutVertices;

	// Public functions
public:
	Renderer();
	void run();

	// Private functions
private:
	void init();
	void pollEvents();
	void update();
	void render();

private:
	Point2D project(Point3D point);
	Point3D rotateX(Point3D point);
	Point3D rotateY(Point3D point);
	Point3D rotateZ(Point3D point);
};