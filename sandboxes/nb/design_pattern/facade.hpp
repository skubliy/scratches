/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   facade.hpp
 * Author: someone
 *
 * Created on June 20, 2016, 11:00 AM
 */

#ifndef FACADE_HPP
#define FACADE_HPP

#include <iostream>

class AHouseBuilder {
    const char* name;

public:

    AHouseBuilder(const char* name) {
        this->name = name;
    }

    virtual ~AHouseBuilder() { }

protected:

    void doJob(const char* text) {
        std::cout << this->name << " will " << text << "." << std::endl;
    }
};
/*
class Bricklayer : public AHouseBuilder {
public:

    Bricklayer(const char* name) : AHouseBuilder(name) { }

    void makeWalls() {
        doJob("make walls");
    }

};

class Roofer : public AHouseBuilder {
public:

    Roofer(const char* name) : AHouseBuilder(name) { }

    void makeRoof() {
        doJob("make roof");
    }

};

class Joiner : public AHouseBuilder {
public:

    Joiner(const char* name) : AHouseBuilder(name) { }

    void installDoors() {
        doJob("install doors");
    }
    
    void installWindows() {
        doJob("install windows");
    }

};
*/

class Bricklayer : public AHouseBuilder {
public:

    Bricklayer() : AHouseBuilder("bricklayer") { }

    void makeWalls() {
        doJob("make walls");
    }

};

class Roofer : public AHouseBuilder {
public:

    Roofer() : AHouseBuilder("roofer") { }

    void makeRoof() {
        doJob("make roof");
    }

};

class Joiner : public AHouseBuilder {
public:

    Joiner() : AHouseBuilder("joiner") { }

    void installDoors() {
        doJob("install doors");
    }
    
    void installWindows() {
        doJob("install windows");
    }

};

class PlaneJobToBuildHouse{
    Bricklayer bricklayer;
    Roofer roofer;
    Joiner joiner;
public:
    PlaneJobToBuildHouse(){}
    
    void whatWillBeDone(){
        bricklayer.makeWalls();
        roofer.makeRoof();
        joiner.installWindows();
        joiner.installDoors();
    
    }
};

void test_facade(){
    PlaneJobToBuildHouse plan;
    plan.whatWillBeDone();
}

#endif /* FACADE_HPP */

