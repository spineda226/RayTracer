#include "Camera.h"
#include "Parse.h"
#include <iostream>
#include <string.h>
#include <sstream>
using namespace std;

Camera::Camera(vec3 location, vec3 up, vec3 right, vec3 look_at):
   location(location),
   up(up),
   right(right),
   look_at(look_at)
{
}

Camera *Camera::parse(ifstream &infile, stringstream &s)
{
	Camera *camera = new Camera();
	cout << "parsing camera" << endl;
	string word, line;
	s >> word;
	if (strcmp(word.c_str(), "{") != 0)
		cerr << "Expected '{' but found '" << word << "'" << endl;

	// Location
	s.clear();
	getline(infile, line);
	s.str(line);
	s >> word;
    //cout << "word: " << word << endl;
	if (strcmp(word.c_str(), "location") != 0)
		cerr << "Expected 'location' but found '" << word << "'" << endl;
	else
		camera->setLoc(Parse::Vector(s));

	// Up
	getline(infile, line);
	s.str(line);
	s >> word;
    //cout << "word: " << word << endl;
	if (strcmp(word.c_str(), "up") != 0)
		cerr << "Expected 'up' but found '" << word << "'" << endl;
	else
		camera->setUp(Parse::Vector(s));

	// Right
	getline(infile, line);
	s.str(line);
	s >> word;
    //cout << "word: " << word << endl;
	if (strcmp(word.c_str(), "right") != 0)
		cerr << "Expected 'right' but found '" << word << "'" << endl;
	else
		camera->setRight(Parse::Vector(s));

	// Look At
	getline(infile, line);
	s.str(line);
	s >> word;
    //cout << "word: " << word << endl;
	if (strcmp(word.c_str(), "look_at") != 0)
		cerr << "Expected 'right' but found '" << word << "'" << endl;
	else
		camera->setLA(Parse::Vector(s));

	// Ending curly brace
	getline(infile, line);
	s.str(line);
	s >> word;
	if (strcmp(word.c_str(), "}") != 0)
		cerr << "Expected '}' but found '" << word << "'" << endl;

	return camera;
}
