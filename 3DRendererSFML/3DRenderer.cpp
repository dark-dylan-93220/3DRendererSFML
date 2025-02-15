#include "3DRenderer.h"

Renderer::Renderer() : 
	window(sf::VideoMode(1200, 700), "3D Renderer", sf::Style::Default)
{
	colorIndex = 0;
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
		L'initialisation c'est pour donner des coordonnées aux points 3D.               (<forme>Points3D)
		Puis les arrêtes sont des Vertex qui relient les "coins" de la forme entre eux. (<forme>Vertices)
	*/

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
	// Diamond pyramid points
	diamondPyramidPoints3D =
	{
		// Top layer
		Point3D{ 0.f, 1.f,  0.f }, 
		// Middle layer
		Point3D{ -1.f, -1.f, 1.f  }, Point3D{ 1.f, -1.f, 1.f }, Point3D{ 1.f, -1.f, -1.f }, Point3D{ -1.f, -1.f, -1.f }, 
		// Bottom layer
		Point3D{ 0.f, -3.f, 0.f }
	};
	// Diamond pyramid vertices
	diamondPyramidVertices =
	{
		Vertex { 0, 1 }, Vertex { 0, 2 },
		Vertex { 0, 3 }, Vertex { 0, 4 },
		Vertex { 1, 2 }, Vertex { 2, 3 },
		Vertex { 3, 4 }, Vertex { 4, 1 },
		Vertex { 1, 5 }, Vertex { 2, 5 },
		Vertex { 3, 5 }, Vertex { 4, 5 }
	};
	// Diamond points
	diamondPoints3D =
	{
		// Bottom layer
		Point3D{ 0.f, -3.f,  0.f  }, 
		// Middle layer
		Point3D{ -1.f, -1.f, 1.f  }, Point3D{ 1.f, -1.f, 1.f },  Point3D{ 1.f, -1.f, -1.f }, Point3D{ -1.f, -1.f, -1.f },
		// Top layer
		Point3D{ -0.5f, 0.f, 0.5f }, Point3D{ 0.5f, 0.f, 0.5f }, Point3D{ 0.5f, 0.f, -0.5f }, Point3D{ -0.5f, 0.f, -0.5f },
	};
	// Diamond vertices
	diamondVertices =
	{
		// Bottom to middle layer
		Vertex { 0, 1 }, Vertex { 0, 2 },
		Vertex { 0, 3 }, Vertex { 0, 4 },
		// Middle points between each other
		Vertex { 1, 2 }, Vertex { 2, 3 },
		Vertex { 3, 4 }, Vertex { 4, 1 },
		// Middle to top layer
		Vertex { 1, 5 }, Vertex { 2, 6 },
		Vertex { 3, 7 }, Vertex { 4, 8 },
		// Top layer between each other
		Vertex { 5, 6 }, Vertex { 6, 7 },
		Vertex { 7, 8 }, Vertex { 8, 5 },
	};
	// Squared donut
	squaredDonutPoints3D =
	{
		// Bottom layer
			// Outer points
		Point3D { -2.f, -0.3f, -2.f }, Point3D { 2.f, -0.3f, -2.f }, Point3D { -2.f, -0.3f, 2.f }, Point3D { 2.f, -0.3f, 2.f },
			// Inner points
		Point3D { -1.f, -0.3f, -1.f }, Point3D { 1.f, -0.3f, -1.f }, Point3D { -1.f, -0.3f, 1.f }, Point3D { 1.f, -0.3f, 1.f },
		// Top layer
			// Outer points
		Point3D { -2.f, 0.3f, -2.f },  Point3D { 2.f, 0.3f, -2.f },  Point3D { -2.f, 0.3f, 2.f },  Point3D { 2.f, 0.3f,  2.f  },
			// Inner points
		Point3D { -1.f, 0.3f, -1.f },  Point3D { 1.f, 0.3f, -1.f },  Point3D { -1.f, 0.3f, 1.f },  Point3D { 1.f, 0.3f,  1.f },
	};
	// Squared donut vertices
	squaredDonutVertices =
	{
		// Bottom outer layer between each other
		Vertex { 0, 1 },   Vertex { 0, 2 },   Vertex { 2, 3 },   Vertex { 1, 3 },
		// Bottom inner layer between each other
		Vertex { 4, 5 },   Vertex { 4, 6 },   Vertex { 6, 7 },   Vertex { 5, 7 },
		// Bottom outer layer to top outer layer
		Vertex { 0, 8 },   Vertex { 1, 9 },   Vertex { 2, 10 },  Vertex { 3, 11 },
		// Bottom inner layer to top inner layer
		Vertex { 4, 12 },  Vertex { 5, 13 },  Vertex { 6, 14 },  Vertex { 7, 15 },
		// Top outer layer between each other
		Vertex { 8, 9 },   Vertex { 8, 10 },  Vertex { 10, 11 }, Vertex { 9, 11 },
		// Top inner layer between each other
		Vertex { 12, 13 }, Vertex { 12, 14 }, Vertex { 14, 15 }, Vertex { 13, 15 }
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

	// Cube

	for (auto& vertex : cubeVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(cubePoints3D[vertex.start]));
		Point3D rotatedPointEnd   = rotateX(rotateY(cubePoints3D[vertex.end]));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color =  sf::Color::White;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White;

		window.draw(line);
	}


	// Diamond pyramid

	for (auto& vertex : diamondPyramidVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(diamondPyramidPoints3D[vertex.start]));
		Point3D rotatedPointEnd = rotateX(rotateY(diamondPyramidPoints3D[vertex.end]));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color = sf::Color::White;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White;

		window.draw(line);
	}


	// Pyramide

	for (auto& vertex : pyramidVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(pyramidPoints3D[vertex.start]));
		Point3D rotatedPointEnd   = rotateX(rotateY(pyramidPoints3D[vertex.end]));

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


	// Diamond

	for (auto& vertex : diamondVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(diamondPoints3D[vertex.start]));
		Point3D rotatedPointEnd = rotateX(rotateY(diamondPoints3D[vertex.end]));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color = sf::Color::White;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White;

		window.draw(line);
	}
	

	// Squared donut

	for (auto& vertex : squaredDonutVertices) {
		// Rotation sur les axes X et Y des points 3D.
		Point3D rotatedPointStart = rotateX(rotateY(squaredDonutPoints3D[vertex.start]));
		Point3D rotatedPointEnd = rotateX(rotateY(squaredDonutPoints3D[vertex.end]));

		// Projection des points de départ et d'arrivée des points 3D en points 2D
		Point2D start = project(rotatedPointStart);
		Point2D end = project(rotatedPointEnd);

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(start.x, start.y);
		line[0].color = sf::Color::White;
		line[1].position = sf::Vector2f(end.x, end.y);
		line[1].color = sf::Color::White;

		window.draw(line);
	}

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