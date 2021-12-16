

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	void uploadFiles(const string& fileName, const vector<string>& text){
        std::ofstream file;
        file.open (fileName);
        for(const string& line : text){
            file << line + "\n";
        }

        file.close();
    }
};

class standardIO:public DefaultIO {
    string read() override {
        string input;
        cin >> input;
        return input;
    }

    void write(string text) override {
        cout << text;
    }

    void write(float f) override {
        cout << f;
    }

    void read(float* f) override {
        cin >> *f;
    }
};

class socketIO:public DefaultIO {
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};


// you may edit this class
class Command{
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes



#endif /* COMMANDS_H_ */
