/**
This application renders a textured mesh that was loaded with Assimp.
*/
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <memory>
#include <SFML/Audio.hpp>
#include "glad.h"

#include "Mesh3D.h"
#include "Object3D.h"
#include "AssimpImport.h"
#include "Animator.h"
#include "Animation.h"
#include "ShaderProgram.h"

/**
 * @brief Defines a collection of objects that should be rendered with a specific shader program.
 */
struct Scene {
	ShaderProgram defaultShader;
	std::vector<Object3D> objects;
	std::vector<Animator> animators;
};

/**
 * @brief Constructs a shader program that renders textured meshes in the Phong reflection model.
 * The shaders used here are incomplete; see their source codes.
 * @return 
 */
ShaderProgram phongLighting() {
	ShaderProgram program;
	try {
		program.load("shaders/light_perspective.vert", "shaders/lighting.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		exit(1);
	}
	return program;
}

/**
 * @brief Constructs a shader program that renders textured meshes without lighting.
 */
ShaderProgram textureMapping() {
	ShaderProgram program;
	try {
		program.load("shaders/texture_perspective.vert", "shaders/texturing.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		exit(1);
	}
	return program;
}

/**
 * @brief Loads an image from the given path into an OpenGL texture.
 */
Texture loadTexture(const std::filesystem::path& path, const std::string& samplerName = "baseTexture") {
	sf::Image i;
	i.loadFromFile(path.string());
	return Texture::loadImage(i, samplerName);
}

/**
 * @brief  Demonstrates loading a square, oriented as the "floor", with a manually-specified texture
 * that does not come from Assimp.
 * @return 
 */
Scene marbleSquare() {
	std::vector<Texture> textures = {
		loadTexture("models/White_marble_03/Textures_4K/white_marble_03_4k_baseColor.tga", "baseTexture"),
	};

	auto mesh = Mesh3D::square(textures);
	auto square = Object3D(std::vector<Mesh3D>{mesh});
	square.grow(glm::vec3(5, 5, 5));
	square.rotate(glm::vec3(-3.14159 / 4, 0, 0));
	return Scene{
		phongLighting(),
		{square}
	};
}

/**
 * @brief Constructs a scene of the textured Stanford bunny.
 */
Scene bunny() {
	auto bunny = assimpLoad("models/bunny_textured.obj", true);
	bunny.grow(glm::vec3(9, 9, 9));
	bunny.move(glm::vec3(0.2, -1, 0));

	return Scene{
		phongLighting(),
		{bunny}
	};
}

/**
 * @brief Constructs a scene of a tiger sitting in a boat, where the tiger is the child object
 * of the boat.
 * @return 
 */
Scene lifeOfPi() {
	// This scene is more complicated; it has child objects, as well as animators.
	auto boat = assimpLoad("./boat/boat.fbx", true);
	boat.move(glm::vec3(0, -0.7, 0));
	boat.grow(glm::vec3(0.01, 0.01, 0.01));
	auto tiger = assimpLoad("./tiger/scene.gltf", true);
	tiger.move(glm::vec3(0, -5, 10));
	boat.addChild(std::move(tiger));
	
	// Because boat and tiger are local variables, they will be destroyed when this
	// function terminates. To prevent that, we need to move them into a vector, and then
	// move that vector as part of the return value.
	std::vector<Object3D> objects;
	objects.push_back(std::move(boat));
	
	// We want these animations to referenced the *moved* objects, which are no longer
	// in the variables named "tiger" and "boat". "boat" is now in the "objects" list at
	// index 0, and "tiger" is the index-1 child of the boat.
	Animator animBoat;
	animBoat.addAnimation(std::make_unique<RotationAnimation>(objects[0], 10, glm::vec3(0, 6.28, 0)));
	Animator animTiger;
	animTiger.addAnimation(std::make_unique<RotationAnimation>(objects[0].getChild(0), 10, glm::vec3(0, 0, 6.28)));

	// The Animators will be destroyed when leaving this function, so we move them into
	// a list to be returned.
	std::vector<Animator> animators;
	animators.push_back(std::move(animBoat));
	animators.push_back(std::move(animTiger));

	// Transfer ownership of the objects and animators back to the main.
	return Scene{
		//phongLighting(),
		//{tiger}
		//textureMapping(),
		//std::move(objects),
		//std::move(animators)
	};
}

Scene skull() {
	//MOUNTAINS?
	auto mountains = assimpLoad("./snow_mountain/Snow_Mountain.obj", true);
	mountains.move(glm::vec3(0, -5, -20));
	mountains.grow(glm::vec3(25));

	//SKULL 
	auto skull = assimpLoad("./skull/skull/skull.obj", true);
	skull.move(glm::vec3(0, 0, -15.0));
	skull.setOrientation(glm::vec3(4.5, 9.5, 9.5));
	skull.grow(glm::vec3(0.2, 0.2, 0.2));

	//EYEBALL 1
	auto eye1 = assimpLoad("./human_eye/Human Eye.obj", true);
	eye1.move(glm::vec3(-4, -15, 14.5));
	eye1.grow(glm::vec3(1.75, 1.75, 1.75));
	eye1.setOrientation(glm::vec3(0, 0, 0));
	skull.addChild(std::move(eye1));

	//EYEBALL 2
	auto eye2 = assimpLoad("./human_eye/Human Eye.obj", true);
	eye2.move(glm::vec3(4, -15, 14.5)); 
	eye2.grow(glm::vec3(1.75, 1.75, 1.75));
	eye2.setOrientation(glm::vec3(0, 0, 0));
	skull.addChild(std::move(eye2));

	//ADD OBJECTS TO SCENE
	std::vector<Object3D> objects;
	objects.push_back(std::move(skull));
	objects.push_back(std::move(mountains));
	
	/*
	note: skull children : 
	0 - lower skull(jaw bone), 
	1 - upper teeth, 
	2 - lower teeth, 
	3 - upper skull, 
	4 - right eye ball / socket
	5 - left eye ball / socket
	*/


	//ANIMATOR 
	Animator animSkull;
	//animSkull.addAnimation(std::make_unique<RotationAnimation>(objects[0], 10, glm::vec3(0, 0, 6)));
	Animator animEye1;
	//animEye1.addAnimation(std::make_unique<RotationAnimation>(objects[0].getChild(5), 10, glm::vec3(0, 15, 6)));
	Animator animEye2;
	//animEye2.addAnimation(std::make_unique<RotationAnimation>(objects[0].getChild(2), 10, glm::vec3(0, 0, 6)));
	//animEye2.addAnimation(std::make_unique<BezierTranslationAnimation>(objects[0].getChild(5), 10, glm::vec3(1, 0, 9), glm::vec3(9, 0, 1), glm::vec3(-1, 0, 9), glm::vec3(0, 0, 10)));
	std::vector<Animator> animators;
	animators.push_back(std::move(animSkull));
	animators.push_back(std::move(animEye1));
	animators.push_back(std::move(animEye2));

	return Scene{
		//phongLighting(),{skull}
		textureMapping(),
		std::move(objects),
		std::move(animators),
	};
}

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::RenderWindow window(sf::VideoMode{ 1600, 1600 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	//"C:\Users\liminal\Desktop\FinalProject449\Lone - Pulsar.mp3"
	//adding music
	sf::Music music;

	if (!music.openFromFile("./Lone - Pulsar.mp3")) {
		return -1;
	}

	music.play();

	// Initialize scene objects.
	//auto scene = lifeOfPi();
	auto scene2 = skull();
	auto& skull = scene2.objects[0];
	auto& jaw = skull.getChild(0);
	auto& topTeeth = skull.getChild(1);
	auto& botTeeth = skull.getChild(2);
	auto& calvaria = skull.getChild(3);
	auto& eye1 = skull.getChild(4);
	auto& eye2 = skull.getChild(5);

	//OBJECT VELOCITY:
	//skull.setVelocity(glm::vec3(0.0, -0.0075, -1.0));
	jaw.setVelocity(glm::vec3(0.0, -0.5, -1.0));
	calvaria.setVelocity(glm::vec3(0.0, 0.5, -1.0));
	eye1.setVelocity(glm::vec3(-1.0, 1, -0.075));
	eye2.setVelocity(glm::vec3(1.0, 1, -0.075));
	topTeeth.setVelocity(glm::vec3(-1.0, -1, -0.075));
	botTeeth.setVelocity(glm::vec3(1.0, 1, -0.075));

	//eye2.setVelocity(glm::vec3(2.0, 0.5, 0.0));
	//auto& mountains = scene2.objects[1];
	//auto& boat = scene2.objects[2];
	

	//auto scene3 = bunny();
	//auto& bunny1 = scene3.objects[0];

	// In case you want to manipulate the scene objects directly by name.
	//auto& boat = scene.objects[0];
	//auto& tiger = boat.getChild(1);

	auto cameraPosition = glm::vec3(0, 0, 0); 
	auto camera = glm::lookAt(cameraPosition, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	auto perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);

	ShaderProgram& mainShader = scene2.defaultShader;
	//ShaderProgram& subShader = scene2.defaultShader;

	mainShader.activate();
	//subShader.activate();
	mainShader.setUniform("view", camera);
	//subShader.setUniform("view", camera);
	mainShader.setUniform("projection", perspective);
	//subShader.setUniform("projection", perspective);

	//std::cout << scene2.objects[0].getCenter() << std::endl;
	//std::cout << scene2.objects[0].getChild(1).getCenter() << std::endl;
	// Ready, set, go!
	for (auto& animator : scene2.animators) {
		animator.start();
	}
	bool running = true;
	sf::Clock c;

	auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}
		//std::cout << skull.getCenter() << std::endl;
		//std::cout << eye1.getCenter() << std::endl;
		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		last = now;

		//eye1.render(window, mainShader);
		eye1.tick(diffSeconds);
		eye2.tick(diffSeconds);
		topTeeth.tick(diffSeconds);
		botTeeth.tick(diffSeconds);
		jaw.tick(diffSeconds);
		calvaria.tick(diffSeconds);
		//skull.tick(diffSeconds);
		for (auto& animator : scene2.animators) {
			animator.tick(diffSeconds);
		}

		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render each object in the scene.
		for (auto& o : scene2.objects) {
			o.render(window, mainShader);
			//o.render(window, subShader);
		}
		window.display();
	}

	return 0;
}


