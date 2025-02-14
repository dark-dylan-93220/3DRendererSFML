#include "3DRenderer.h"

Renderer::Renderer() : 
	window(sf::VideoMode(800, 600), "3D Renderer", sf::Style::Default)
{
	window.setFramerateLimit(60);
	event = {};
	rotation = 0.0f;
	deltaTime = 0.0f;
	FOV = 10.f;
	SCALE = window.getSize().x / 15;
}

void Renderer::run() {

	init();

	while (window.isOpen()) {
		pollEvents();
		update();
		render();
	}
}

void Renderer::init() {

	/*
		Note :
		L'initialisation c'est pour donner des coordonnées aux points 3D.
		Puis les arrêtes sont des Vertex qui relient les "coins" de la forme entre eux
	*/

	// Rainbow colors
	// (Red, Orange, Yellow, Green, Blue, Purple)
	rainbowColors =
	{
		sf::Color(255, 0,   0),sf::Color(255, 128, 0),
		sf::Color(255, 255, 0),sf::Color(0,   128, 0),
		sf::Color(0,   0, 255),sf::Color(128, 0, 255)
	};

	// Cube points
	cubePoints3D = 
	{
		Point3D{ -1.f, -1.f, -1.f }, Point3D{ -1.f, -1.f, 1.f  }, Point3D{ 1.f, -1.f, -1.f },
		Point3D{ -1.f, 1.f,  -1.f }, Point3D{ -1.f, 1.f,  1.f  }, Point3D{ 1.f, -1.f, 1.f  },
		Point3D{ 1.f,  1.f,  -1.f }, Point3D{ 1.f,  1.f,  1.f  }
	};
	// Cube vertices
	cubeVertices = 
	{
		Vertex { 0, 1 }, Vertex { 0, 2 }, Vertex { 0, 3 },
		Vertex { 2, 5 }, Vertex { 3, 6 }, Vertex { 3, 4 },
		Vertex { 4, 7 }, Vertex { 6, 7 }, Vertex { 7, 5 },
		Vertex { 5, 1 }, Vertex { 4, 1 }, Vertex { 2, 6 }
	};
	// Pyramid points
	pyramidPoints3D =
	{
		Point3D{ 0.f, 1.f,  0.f  }, Point3D{ -1.f, -1.f, 1.f  }, Point3D{ 1.f, -1.f, 1.f },
		Point3D{ 1.f, -1.f, -1.f }, Point3D{ -1.f, -1.f, -1.f }
	};
	// Pyramid vertices
	pyramidVertices =
	{
		Vertex { 0, 1 }, Vertex { 0, 2 },
		Vertex { 0, 3 }, Vertex { 0, 4 },
		Vertex { 1, 2 }, Vertex { 2, 3 },
		Vertex { 3, 4 }, Vertex { 4, 1 }
	};
}

void Renderer::pollEvents() {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}
}

void Renderer::update() {
	deltaTime = Clock.restart().asSeconds();
	rotation += deltaTime;
}

void Renderer::render() {
	window.clear();

	if (colorIndex >= 6) { colorIndex = 0; }

	// Cube
	for (auto& vertex : cubeVertices) {
		// Rotation sur les axes X et Y des points 3D.
		// (Enlever les commentaires pour activer la rotation autour de l'axe Z)
		Point3D rotatedPointStart = rotateX(rotateY(/*rotateZ(*/cubePoints3D[vertex.start]/*)*/));
		Point3D rotatedPointEnd   = rotateX(rotateY(/*rotateZ(*/cubePoints3D[vertex.end]  /*)*/));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color =  sf::Color::White /*rainbowColors[(int)colorIndex]*/;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White /*rainbowColors[(int)colorIndex]*/;

		window.draw(line);
	}
	colorIndex+=(deltaTime * 5.f);

	// Pyramide
	/*
	for (auto& vertex : pyramidVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(rotateZ(pyramidPoints3D[vertex.start])));
		Point3D rotatedPointEnd   = rotateX(rotateY(rotateZ(pyramidPoints3D[vertex.end])));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end   = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color = sf::Color::White;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White;

		window.draw(line);
	}
	*/

	window.display();
}

/*
	Note :
	Les fonctions de rotations sur les axes X, Y et Z sont données par le produit
	de la matrice colonne {X, Y, Z} et des matrices de rotations sur leur axe respectif.
*/

// Rotation autour de l'axe X

Point3D Renderer::rotateX(Point3D point) {
	return Point3D{
		point.x,
		point.y * cos(rotation) - point.z * sin(rotation),
		point.y * sin(rotation) + point.z * cos(rotation)
	};
}

// Rotation autour de l'axe Y

Point3D Renderer::rotateY(Point3D point) {
	return Point3D{
		point.x * cos(rotation) + point.z * sin(rotation),
		point.y,
		-point.x * sin(rotation) + point.z * cos(rotation)
	};
}

// Rotation autour de l'axe Z

Point3D Renderer::rotateZ(Point3D point) {
	return Point3D{
		point.x * cos(rotation) - point.y * sin(rotation),
		point.x * sin(rotation) + point.y * cos(rotation),
		point.z
	};
}

// Projection d'un point 3D vers un point 2D

Point2D Renderer::project(Point3D point) {
	return Point2D{
		window.getSize().x / 2 + (point.x * FOV) / (point.z + FOV) * SCALE,
		window.getSize().y / 2 + (point.y * FOV) / (point.z + FOV) * SCALE
	};
}