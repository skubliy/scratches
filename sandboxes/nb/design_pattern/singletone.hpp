/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   singletone.hpp
 * Author: someone
 *
 * Created on June 21, 2016, 3:09 PM
 */

#ifndef SINGLETONE_HPP
#define SINGLETONE_HPP

class Singleton_static 
{
public:
    static Singleton_static& instance()
    {
        static Singleton_static instance;
        return instance;
    }
protected:
    Singleton_static();
    Singleton_static(const Singleton_static &classRef);
    Singleton_static& operator=(const Singleton_static &classRef);
};


#endif /* SINGLETONE_HPP */

