#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <cmath>

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
	std::vector<Vertex> cubeVertices;
	std::vector<Vertex> pyramidVertices;

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