

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include <algorithm>
#include <vector>

using namespace std;

class CLI {
	DefaultIO* dio;
    vector<Command*> commands;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();

};

#endif /* CLI_H_ */
