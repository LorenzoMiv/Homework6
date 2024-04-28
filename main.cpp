#include <SFML/Graphics.hpp>
#include <iostream>

#include "Mesh3D.h"
#include "Object3D.h"
#include <filesystem>

#include <memory>
#include <glm/ext.hpp>
#include <assimp/scene.h>
#include <cmath>
#include "math.h"
#include "AssimpImport.h"
#include "Animator.h"


void drawPixel(sf::RenderWindow& window, sf::Vector2u position, sf::Color color) {
	sf::Vertex pixel[1] = {
		sf::Vertex(sf::Vector2f(position.x, position.y), color)
	};
	window.draw(pixel, 1, sf::Points);
}

int main() {
	sf::RenderWindow window{ sf::VideoMode{1200, 800}, "SFML Demo" };
	sf::Clock c;



	//TEST BUNNY:
	auto bunny = assimpLoad("C:/Users/loren/Homework6/models/bunny.obj");
	// TEST SQUARE:
	auto square = Object3D(std::make_shared<Mesh3D>(Mesh3D::house(sf::Color::Red)));

	auto square2 = Object3D(std::make_shared<Mesh3D>(Mesh3D::square(sf::Color::Green)));

	// Use GLM to create a perspective-projection matrix with a 45deg FOVY.
	auto proj = glm::perspective(glm::radians(45.0), (double)window.getSize().x / window.getSize().y, 0.1, 100.0);

	//rotation animation: 
	auto anim = Animator();
	anim.addAnimation(std::make_unique<BezierTranslationAnimation>(square, 3, glm::vec3(-2.0, -2.0, -6.0), glm::vec3(2.0, -2.0, -3.0), glm::vec3(-2.0, -2.0, 3.0), glm::vec3(2.0, 0.0, -3.0)));
	anim.addAnimation(std::make_unique<TranslationAnimation>(square, 2, glm::vec3(-5.0, 2.5, 0)));
	anim.start();

	auto anim2 = Animator();
	anim2.addAnimation(std::make_unique<BezierTranslationAnimation>(square2, 3, glm::vec3(-2.0, 2.0, 6.0), glm::vec3(2.0, -2.0, 3.0), glm::vec3(-2.0, 2.0, -3.0), glm::vec3(2.0, 0.0, 3.0)));
	anim2.addAnimation(std::make_unique<PauseAnimation>(square2, 2.5));
	anim2.addAnimation(std::make_unique<RotationAnimation>(square2, 1, glm::vec3(0.0, 0.0, 2.0 * acos(-1.0))));

	anim2.start();

	//auto anim3 = Animator();
	//anim3.addAnimation(std::make_unique<BezierTranslationAnimation>(square2, 1, glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, -1.0, 0.5), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.5, 0.5, 0.5)));
	//anim3.start();

	// Use an identity matrix for the camera, which places it at (0, 0, 0) looking down the negative z axis.
	// TODO: learn how to use glm::lookAt, to place the camera at (3, 0, 3) looking at the origin.
	glm::vec3 cameraPos(3.0, 0.0, 3.0);
	glm::vec3 targetPos(0.0, 0.0, -1.0);
	glm::vec3 upVec(0.0, 0.00001, 0.0);

	glm::mat4 view = glm::lookAt(cameraPos, targetPos, upVec);


	// The square in local space is at the origin; move it away from the camera so we can see it.

	//SQUARE TEST:
	square.move(glm::vec3(0, 0, -3));

	//square2.move(glm::vec3(0, 0, -1));

	//BUNNY TEST: 

	bunny.move(glm::vec3(0.2, -1.0, -3.0));
	bunny.grow(glm::vec3(9.0));

	square2.setVelocity(glm::vec3(0.0, 0.5, 0.0));
	square2.setAcceleration(glm::vec3(5.0, -0.5, 0.0));
	square2.setRotAcceleration(glm::vec3(0.0, 1.0, 0.0));
	square2.setRotVelocity(glm::vec3(2.0, 0, 0));

	auto last = c.getElapsedTime();
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		//SQUARE TEST: 
		//square.rotate(glm::vec3(0.001, 0, 0));
		//square2.setVelocity(glm::vec3(0.0, 0.0, 3.0));




		//square.render(window, view, proj);
		square2.render(window, view, proj);



		//BUNNY TEST:
		//bunny.rotate(glm::vec3(0.0, 0.001, 0.0));
		//bunny.render(window, view, proj);

		window.display();

		auto now = c.getElapsedTime();
		auto diff = now - last;
		//anim2.tick(diff.asSeconds());
		//square2.tick(diff.asSeconds());
		
		if (square2.getPosition().x > 2) {
			square2.setVelocity(-square2.getVelocity());
		}

		if (square2.getPosition().x < -3) {
			square2.setVelocity(-square2.getVelocity());
		}


		square2.tick(diff.asSeconds());
		
		//std::cout << "x: " << square2.getPosition().x << ", " << "z: " << square2.getPosition().z << std::endl;
		
		
		

		anim.tick(diff.asSeconds());
		//anim3.tick(diff.asSeconds());
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		last = now;
	}

	return 0;
}



