
#include "Parse.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Box.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string.h>
#include <stdlib.h> //strtof
//#include <easy/profiler.h>
using namespace std;

AABB *Parse::parse_file(string filename, std::vector<Triangle *> *triangles, std::vector<Shape *> *planes, Camera **camera, std::vector<LightSource *> *lights)
{
	//EASY_FUNCTION(profiler::colors::Magenta); // Magenta block with name "foo"
	stringstream s;
	string word;
	string line;
	ifstream infile(filename);
	AABB *boundingBox = new AABB();
	bool firstTri = true;
	if (infile.fail())
	{
		cerr << "file not found: " << filename << endl;
		exit(EXIT_FAILURE);
	}
	while (getline(infile, line)) {
		s.clear();
		s.str(line); // creates a stream from the line 
		s >> word;
		const char *firstword = word.c_str();
		//cout << "FIRST WORD: " << firstword << endl;
		if (strcmp(firstword, "//") == 0 || strcmp(firstword, "") == 0)
		{
			//cout << "comment or empty" << endl;
		}
		else if (strcmp(firstword, "light_source") == 0)
			lights->push_back(LightSource::parse(infile, s));
		else if (strcmp(firstword, "camera") == 0)
			*camera = Camera::parse(infile, s);
		//else if (strcmp(firstword, "sphere") == 0)
		//	objects->push_back(Sphere::parse(infile, s));
		//else if (strcmp(firstword, "plane") == 0)
		//	planes->push_back(Plane::parse(infile, s));
		else if (strcmp(firstword, "triangle") == 0) {
			Triangle *t = Triangle::parse(infile, s);
			if (firstTri) {
				boundingBox->reset(t->getP1());
				firstTri = false;
			}
			else {
				boundingBox->addPoint(t->getP1());
			}
			boundingBox->addPoint(t->getP2());
			boundingBox->addPoint(t->getP3());
			triangles->push_back(t);
		}
		//else if (strcmp(firstword, "box") == 0)
		//	objects->push_back(Box::parse(infile, s));
		else
		{
			cout << "did not parse: " << firstword << endl;
		}
		word = "";		
	}
	return boundingBox;
}

vec3 Parse::Vector(stringstream & Stream)
{
	vec3 v;
	v.x = v.y = v.z = 0.f;
	stringbuf buf;

	Stream.ignore(numeric_limits<streamsize>::max(), '<');
	Stream.get(buf, '>');
	Stream.ignore(numeric_limits<streamsize>::max(), '>');

	string line = buf.str();
	//cout << line << endl;
	int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);

	if (read != 3)
	{
		cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
	}
	return v;
}

vec4 Parse::Vector4(stringstream & Stream)
{
	vec4 v;
	v.x = v.y = v.z = 0.f;
	stringbuf buf;

	Stream.ignore(numeric_limits<streamsize>::max(), '<');
	Stream.get(buf, '>');
	Stream.ignore(numeric_limits<streamsize>::max(), '>');

	string line = buf.str();
	//cout << line << endl;
	int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.w);

	if (read != 4)
	{
		cerr << "Expected to read 4 vector elements but found '" << line << "'" << endl;
	}
	return v;
}

void Parse::Modifiers(Shape *shape, ifstream &infile)
{
	stringstream s;
	string word, line;
	getline(infile, line);
	s.str(line);
	s >> word;
	const char *firstword = word.c_str();
	while (strcmp(firstword, "}")) // keep looping until you read  '}'
	{
		//cout << "parse modifier word: " << firstword << endl;
		if (strcmp(firstword, "pigment") == 0)
			Parse::Pigment(shape, s);
		else if (strcmp(firstword, "finish") == 0)
			Parse::Finish(shape, s);
		else
		{
			cout << "Skipping: " << firstword << endl;
		}

		getline(infile, line);
		s.clear();
		s.str(line);
		s >> word;
		firstword = word.c_str();
	}
}

void Parse::Pigment(Shape *shape, stringstream &s)
{
	//cout << "parsing pigment" << endl;
	string word;
	s.ignore(numeric_limits<streamsize>::max(), '{');
	s >> word; // color
	s >> word; // rgb or rgbf
	if (strcmp(word.c_str(), "rgb") == 0)
	{
		shape->setColor(Parse::Vector(s));
		shape->setFilter(0);
	}
	else // rgbf
	{
		vec4 rgbf = Parse::Vector4(s);
		shape->setColor(vec3(rgbf.x, rgbf.y, rgbf.z));
		shape->setFilter(rgbf.w);
		cout << "filter: " << rgbf.w << endl;
	}
}

void Parse::Finish(Shape *shape, stringstream &s)
{	
	float value;
	string word = "";
	stringbuf buf;
	const char *w;
	char *leftover = NULL;
	properties *finish = shape->getFinish();
	//cout << "parsing finish" << endl;
	s.ignore(numeric_limits<streamsize>::max(), '{');
	while (strcmp(word.c_str(), "}") && (leftover == NULL || *leftover != '}'))
	{
		s >> word;
		//cout << "word: " <<  word << endl;
		w = word.c_str();
		if (strcmp(w, "ambient") == 0)
		{
			//cout << "parsing ambient" << endl;
			//cout << "Finish ambient before: " << shape->getFinish()->ambient << endl;
			s >> word;
			value= strtof(word.c_str(), &leftover);
			finish->ambient = value;
			//cout << "value: " << word << endl; 
			//cout << "leftover: " << *leftover << endl;
		}
		else if (strcmp(w, "diffuse") == 0)
		{
			//cout << "parsing diffuse" << endl;
			s >> word;
			value = strtof(word.c_str(), &leftover);
			finish->diffuse = value;
			//cout << "value: " << word << endl; 
			//cout << "leftover: " << *leftover << endl;
		}
		else if (strcmp(w, "specular") == 0)
		{
			s >> word;
			finish->specular = strtof(word.c_str(), &leftover);
		}
		else if (strcmp(w, "roughness") == 0)
		{
			s >> word;
			finish->roughness = strtof(word.c_str(), &leftover);
		}
		else if (strcmp(w, "reflection") == 0)
		{
			s >> word;
			finish->reflection = strtof(word.c_str(), &leftover);
		}
		else if (strcmp(w, "refraction") == 0)
		{
			s >> word;
			finish->refraction = strtof(word.c_str(), &leftover);
		}
		else if (strcmp(w, "ior") == 0)
		{
			s >> word;
			finish->ior = strtof(word.c_str(), &leftover);
		}
		else
		{
			cout << "Skipping: " << w << endl;
		}

	}
}

