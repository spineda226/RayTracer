
#include "Parse.h"
#include "Sphere.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string.h>
using namespace std;

void Parse::parse_file(string filename, std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights)
{
	stringstream s;
	string word;
	string line;
	//stringstream s;
	ifstream infile(filename);
	while (getline(infile, line)) {
		s.clear();
		s.str(line); // creates a stream from the line 
		//s.get(buf, ' '); // puts chracters before the first space into buf
		s >> word;
		const char *firstword = word.c_str();
		cout << "FIRST WORD: " << firstword << endl;
		if (strcmp(firstword, "//") == 0 || strcmp(firstword, "") == 0)
		{
			cout << "comment or empty" << endl;
			//continue;
		}
		else if (strcmp(firstword, "light_source") == 0)
		{
			lights->push_back(LightSource::parse(infile, s));
		}
		else if (strcmp(firstword, "camera") == 0)
		{
			*camera = Camera::parse(infile, s);
		}
		else if (strcmp(firstword, "sphere") == 0)
		{
			objects->push_back(Sphere::parse(infile, s));
		}
		else if (strcmp(firstword, "plane") == 0)
		{
			cout << "plane" << endl;
		}
		else
		{
			cout << "other " << endl;
		}
		word = "";
		//cout << firstword << endl;
		//getline(infile, line);
		//cout << line << endl;
		//cout << s.str() << endl;
		
	}
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
	cout << line << endl;
	int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);

	if (read != 3)
	{
		cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
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
		cout << "parse modifier word: " << firstword << endl;
		if (strcmp(firstword, "pigment") == 0)
			Parse::Pigment(shape, s);
		else if (strcmp(firstword, "finish") == 0)
			Parse::Finish(shape, s);
		getline(infile, line);
		s.clear();
		s.str(line);
		s >> word;
		firstword = word.c_str();
	}
}

void Parse::Pigment(Shape *shape, stringstream &s)
{
	cout << "parsing pigment" << endl;
	shape->setColor(Parse::Vector(s));
}

void Parse::Finish(Shape *shape, stringstream &s)
{
	cout << "parsing finish" << endl;
	//shape->setColor(Parse::Vector(s));
}

