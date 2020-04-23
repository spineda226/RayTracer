
#include "Parse.h"
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
			cout << "sphere" << endl;
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

